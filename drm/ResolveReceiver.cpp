#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "DRM.h"
#include "DRMIndex.h"
#include "ResolveObject.h"
#include "ResolveReceiver.h"
#include "ResolveSection.h"
#include "../filesystem/FileSystem.h"
#include "../filesystem/FileUserBufferReceiver.h"

extern "C" {
#include "../miniz/miniz.h"
}

extern char pathPrefix[36];

namespace cdc {

static void applyRelocs(
	ResolveSection **resolveSections,
	DRMSectionHeader *sectionHeaders,
	size_t *sectionDomainIds,
	size_t sectionIndex,
	char *relocData)
{

	ResolveSection* resolveSection = resolveSections[sectionHeaders[sectionIndex].type];

	if (resolveSection == nullptr)
		return;

	char *data = (char*)resolveSection->getBlob(sectionDomainIds[sectionIndex]);

	struct RelocHeader {
		uint32_t count0;
		uint32_t count1;
		uint32_t count2;
		uint32_t count3;
		uint32_t count4;
	} *relocHeader = (RelocHeader*)relocData;

	auto *relocCursor = (uint32_t*)(&relocHeader[1]);

	for (unsigned i=0; i < relocHeader->count0; i++, relocCursor += 2) {
		uint32_t patchSite = relocCursor[0];
		uint32_t targetOffset = relocCursor[1];

		char *patch = data + patchSite;
		char *target = data + targetOffset;
		memcpy(patch, &target, sizeof(void*));
	}

	for (unsigned i=0; i < relocHeader->count1; i++, relocCursor += 2) {
		uint32_t lo = relocCursor[0];
		uint32_t hi = relocCursor[1];
		uint64_t reloc = (((uint64_t)hi)<<32) | lo;
		uint32_t targetIndex = (reloc & 0x0000000000003FFF) >> 00;
		uint32_t patchSite = (reloc & 0x0000003FFFFFC000) >> 12;
		uint32_t targetOffset = (reloc & 0xFFFFFFC000000000) >> 38;

		ResolveSection *targetResolveSection = resolveSections[sectionHeaders[targetIndex].type];
		if (!targetResolveSection) continue; // not in original engine
		char *targetData = (char*)targetResolveSection->getBlob(sectionDomainIds[targetIndex]);
		if (!targetData) continue; // not in original engine
		char *patch = data + patchSite;
		char *target = targetData + targetOffset;
		memcpy(patch, &target, sizeof(void*));
	}

	for (unsigned i=0; i < relocHeader->count2; i++, relocCursor += 1) {
		uint32_t reloc = relocCursor[0];
		uint32_t patchSite = (reloc & 0x01FFFFFF)*4;
		uint32_t targetTy = reloc >> 25;

		char *patch = data + patchSite;
		uint32_t targetSectionId = 0;
		memcpy(&targetSectionId, patch, 4);

		ResolveSection *targetResolveSection = resolveSections[targetTy];
		if (!targetResolveSection) continue; // not in original engine
		uint32_t targetDomainId = targetResolveSection->getDomainId(targetSectionId);

		memcpy(patch, &targetDomainId, 4);
	}

	// type 3 is broken and unused

	for (unsigned i=0; i < relocHeader->count4; i++, relocCursor += 1) {
		uint32_t reloc = relocCursor[0];
		uint32_t patchSite = (reloc & 0x01FFFFFF)*4;
		uint32_t targetTy = reloc >> 25;

		char *patch = data + patchSite;
		uint32_t targetSectionId = 0;
		memcpy(&targetSectionId, patch, 4);

		ResolveSection *targetResolveSection = resolveSections[targetTy];
		if (!targetResolveSection) continue; // not in original engine
		uint32_t targetDomainId = targetResolveSection->getDomainId(targetSectionId);
		void *targetData = nullptr;
		if (targetDomainId != ~0)
			targetData = targetResolveSection->getWrapped(targetDomainId);
		memcpy(patch, &targetData, 4);	
	}
}

static std::vector<char> decompressCDRM(std::vector<char>& data) {
	struct CDRMHeader {
		uint32_t magic;
		uint32_t version;
		uint32_t count;
		uint32_t padding;
	} header;

	memcpy(&header, data.data(), sizeof(CDRMHeader));

	if (header.magic != 0x4D524443)
		return data;

	printf("  decompressing\n");

	size_t cursor = 16 + header.count * 8;
	cursor = (cursor+15) & ~15;
	std::vector<char> output;

	for (uint32_t i = 0; i<header.count; i++) {

		struct CDRMChunk {
			uint32_t type : 8;
			uint32_t unpackedSize : 24;
			uint32_t packedSize;
		} chunk;
		memcpy(&chunk, data.data() + 16 + i*8, 8);

		size_t targetOffset = output.size();
		size_t packedSize = chunk.packedSize;
		size_t unpackedSize = chunk.unpackedSize;
		output.resize(targetOffset + unpackedSize);

		if (chunk.type == 1) {
			memcpy(output.data() + targetOffset, data.data() + cursor, packedSize);
		} else if (chunk.type == 2) {
			unsigned long unpackedSizeActual = unpackedSize;
			uncompress(
				(unsigned char*)(output.data() + targetOffset), &unpackedSizeActual,
				(unsigned char*)(data.data() + cursor), packedSize);
		}

		// align output
		for (auto j = unpackedSize; (j & 15); j++)
			output.push_back(0);

		// align input
		cursor += chunk.packedSize;
		cursor = (cursor+15) & ~15;
	}

	return output;
}

// from ObjectSection.cpp
void objectLoadCallback(void*, void*, void*, ResolveObject* resolveObject);
void objectUnloadCallback(ObjectTracker*, ResolveObject*);

std::vector<DRMSectionHeader> hackResolveReceiver(
	std::vector<char> data,
	ResolveSection **resolveSections,
	ResolveObject *resolveObject,
	bool requestDependencies)
{
	data = decompressCDRM(data);
	DRMHeader header;
	memcpy(&header, data.data(), sizeof(header));
	std::vector<DRMSectionHeader> sectionHeaders(header.sectionCount);
	memcpy(sectionHeaders.data(), data.data() + sizeof(header), sizeof(DRMSectionHeader) * header.sectionCount);

	uint32_t cursor = 32 + header.sectionCount*20;
	uint32_t dependencyDrmListOffset = cursor; cursor += header.dependencyDrmListSize;
	uint32_t dependencyObjListOffset = cursor; cursor += header.dependencyObjListSize;
	cursor = (cursor+15) & ~15;

	if (requestDependencies) {
		char *dep = data.data() + dependencyDrmListOffset;
		char *end = dep + header.dependencyDrmListSize;
		uint32_t missingDeps = 0;
		for (; dep < end; dep += strlen(dep) + 1) {
			char path[512];
			sprintf(path, "%s%s", pathPrefix, dep); // HACK

			auto depResolveObject = ResolveObject::create(
				path,
				&objectLoadCallback,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				0,
				1);
			resolveObject->addDependency(depResolveObject);
			if (!isLoaded(depResolveObject)) {
				printf("  - %s\n", dep);
				missingDeps++;
				depResolveObject->addDependant(resolveObject);
			} else {
				printf("  + %s\n", dep);
			}
		}
		if (missingDeps != 0) {
			// recreate this receiver, for a time when the dependencies have arrived
			/*auto newRr = new ResolveReceiver(
				callback,
				callbackArg1,
				callbackArg2,
				rootSectionPtr,
				unloadCallback,
				objectTracker,
				object,
				unknown,
				index);

			resolveObject->markForRetry(missingDeps, newRr);*/

			return {};
		}
	}

	std::vector<uint32_t> sectionDomainIds;
	std::vector<char*> relocPtrs;

	// sectionHeader.languageBits >> 30 =
	// 1: DX9
	// 2: DX11

	for (uint32_t i = 0; i < header.sectionCount; i++) {
		DRMSectionHeader& sectionHeader = sectionHeaders[i];

		char *reloc = data.data() + cursor;
		cursor += sectionHeader.relocSize;
		cursor = (cursor+15) & ~15;

		char *payload = data.data() + cursor;
		cursor += sectionHeader.payloadSize;
		cursor = (cursor+15) & ~15;

		auto *resolveSection = sectionHeader.type < 16 ? resolveSections[sectionHeader.type] : nullptr;
		if (resolveSection && (sectionHeader.languageBits >> 30) != 1) {
			bool alreadyLoaded = false;
			uint32_t id = resolveSection->allocate(
				sectionHeader.id,
				sectionHeader.allocFlags,
				sectionHeader.unknown06,
				sectionHeader.payloadSize,
				alreadyLoaded);
			printf("  section %3d %2d %04x (%04x)\n", i, sectionHeader.type, sectionHeader.id, id);
			if (!alreadyLoaded) {
				resolveSection->fill(id, payload, sectionHeader.payloadSize, 0);
			}
			sectionDomainIds.push_back(id);
		} else {
			sectionDomainIds.push_back(0);
		}
		relocPtrs.push_back(sectionHeader.relocSize >= 20 ? reloc : nullptr);
	}

	for (uint32_t i = 0; i < header.sectionCount; i++) {
		DRMSectionHeader& sectionHeader = sectionHeaders[i];
		if (relocPtrs[i] && (sectionHeader.languageBits >> 30) != 1)
			applyRelocs(resolveSections, sectionHeaders.data(), sectionDomainIds.data(), i, relocPtrs[i]);
	}

	for (uint32_t i = 0; i < header.sectionCount; i++) {
		DRMSectionHeader& sectionHeader = sectionHeaders[i];
		auto *resolveSection = sectionHeader.type < 16 ? resolveSections[sectionHeader.type] : nullptr;
		if (resolveSection && (sectionHeader.languageBits >> 30) != 1)
			resolveSection->construct(sectionDomainIds[i], nullptr);
	}

	// what the hell is a "read det", what was I thinking?
	auto dets = new DRMReadDet[header.sectionCount];
	resolveObject->drmReadDets = new DRMReadDets;
	resolveObject->drmReadDets->dets = dets;
	resolveObject->drmReadDets->numDets = header.sectionCount;
	resolveObject->rootSection = header.rootSection;
	for (uint32_t i = 0; i < header.sectionCount; i++) {
		dets[i].domainID = sectionDomainIds[i];
		dets[i].contentType = sectionHeaders[i].type;
		dets[i].sectionID = sectionHeaders[i].id;
	}

	return sectionHeaders;
}

void hackResolveReceiver(FileSystem *fs, const char *path, ResolveSection **resolveSections, ResolveObject *resolveObject, DRMIndex *index) {
	printf("loading %s\n", path);

	File *file = fs->createFile(path);
	uint32_t size = file->getSize();
	std::vector<char> buffer(size);
	FileReceiver *rec = FileUserBufferReceiver::create(buffer.data());
	FileRequest *req = file->createRequest(rec, path, 0);
	req->setReadAmount(size);
	req->submit(3);
	req->decrRefCount();
	fs->processAll();
	// req is owned by fs which takes care of it in processAll()
	delete file;

	auto sectionHeaders = hackResolveReceiver(buffer, resolveSections, resolveObject);
	if (index)
		index->sectionHeaders[std::string(path)] = std::move(sectionHeaders);
}

void ResolveReceiver::process(FileRequest*, void *input, uint32_t size, uint32_t offset) {
	if (offset != buffer.size()) {
		fprintf(stderr, "ResolveReceiver::process got random chunk of data %d vs %d\n",
			(int)offset, (int)buffer.size());
		return;
	}

	buffer.resize(buffer.size() + size);
	memcpy(buffer.data() + offset, input, size);
}

void ResolveReceiver::requestFailed(FileRequest *req) {
	// TODO
}

void ResolveReceiver::requestComplete(FileRequest *req) {
	printf("loading %s\n", resolveObject->path);
	auto sectionHeaders = hackResolveReceiver(std::move(buffer), g_resolveSections, resolveObject);

	if (sectionHeaders.empty()) {
		// recreate this receiver, for a time when the dependencies have arrived
		auto newRr = new ResolveReceiver(
			callback,
			callbackArg1,
			callbackArg2,
			rootSectionPtr,
			unloadCallback,
			objectTracker,
			resolveObject,
			0, // unknown,
			index);

		resolveObject->markForRetry(1 /*missingDeps*/, newRr);
		
		req->decrRefCount();
		delete this;
		return;
	}

	req->decrRefCount();
	resolveObject->fileRequest = nullptr;
	resolveObject->resolveReceiver = nullptr;

	resolveObject->notifyDependants();

	void *wrapped = nullptr;
	if (resolveObject->rootSection != ~0u) {
		auto& rootSection = sectionHeaders[resolveObject->rootSection];
		if (g_resolveSections[rootSection.type]) // HACK
			wrapped = g_resolveSections[rootSection.type]->getWrapped(
				resolveObject->drmReadDets->dets[resolveObject->rootSection].domainID // TODO
			);
	}

	if (rootSectionPtr)
		*rootSectionPtr = wrapped;

	if (callback)
		callback(wrapped, callbackArg1, callbackArg2, resolveObject);

	if (index) // custom addition
		index->sectionHeaders[std::string(resolveObject->path)] = std::move(sectionHeaders);

	delete this;
}

}
