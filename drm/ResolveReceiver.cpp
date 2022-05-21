#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "ResolveReceiver.h"
#include "ResolveSection.h"
#include "DRM.h"

extern "C" {
#include "../miniz/miniz.h"
}

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

void hackResolveReceiver(std::vector<char> data, ResolveSection **resolveSections) {
	data = decompressCDRM(data);
	DRMHeader header;
	memcpy(&header, data.data(), sizeof(header));
	std::vector<DRMSectionHeader> sectionHeaders(header.sectionCount);
	memcpy(sectionHeaders.data(), data.data() + sizeof(header), sizeof(DRMSectionHeader) * header.sectionCount);

	uint32_t cursor = 32 + header.sectionCount*20 + header.dependencyDrmListSize + header.dependencyObjListSize;
	cursor = (cursor+15) & ~15;

	std::vector<uint32_t> sectionDomainIds;
	std::vector<char*> relocPtrs;

	for (uint32_t i = 0; i < header.sectionCount; i++) {
		DRMSectionHeader& sectionHeader = sectionHeaders[i];

		char *reloc = data.data() + cursor;
		cursor += sectionHeader.relocSize;
		cursor = (cursor+15) & ~15;

		char *payload = data.data() + cursor;
		cursor += sectionHeader.payloadSize;
		cursor = (cursor+15) & ~15;

		auto *resolveSection = sectionHeader.type < 16 ? resolveSections[sectionHeader.type] : nullptr;
		if (resolveSection) {
			bool alreadyLoaded = false;
			uint32_t id = resolveSection->allocate(
				sectionHeader.id,
				sectionHeader.allocFlags,
				sectionHeader.unknown06,
				sectionHeader.payloadSize,
				alreadyLoaded);
			printf("  section %3d %04x (%04x)\n", i, sectionHeader.id, id);
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
		if (relocPtrs[i])
			applyRelocs(resolveSections, sectionHeaders.data(), sectionDomainIds.data(), i, relocPtrs[i]);
	}

	for (uint32_t i = 0; i < header.sectionCount; i++) {
		DRMSectionHeader& sectionHeader = sectionHeaders[i];
		auto *resolveSection = sectionHeader.type < 16 ? resolveSections[sectionHeader.type] : nullptr;
		if (resolveSection)
			resolveSection->construct(sectionDomainIds[i], nullptr);
	}
}

void hackResolveReceiver(const char *path, ResolveSection **resolveSections) {
	printf("loading %s\n", path);
	FILE *f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	std::vector<char> buffer(size);
	fread(buffer.data(), size, 1, f);
	fclose(f);

	hackResolveReceiver(buffer, resolveSections);
}

}
