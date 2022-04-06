#include <stdint.h>
#include <vector>
#include <string.h>
#include "ResolveReceiver.h"
#include "ResolveSection.h"
#include "DRM.h"

namespace cdc {

static void applyRelocs(
	ResolveSection *resolveSections,
	DRMSectionHeader *sectionHeaders,
	size_t *sectionDomainIds,
	size_t sectionIndex,
	char *relocData)
{

	ResolveSection* resolveSection = &resolveSections[sectionHeaders[sectionIndex].type];

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
		uint32_t hi = relocCursor[0];
		uint64_t reloc = (((uint64_t)hi)<<32) | lo;
		uint32_t targetIndex = (reloc & 0x0000000000003FFF) >> 00;
		uint32_t patchSite = (reloc & 0x0000003FFFFFC000) >> 12;
		uint32_t targetOffset = (reloc & 0xFFFFFFC000000000) >> 38;

		ResolveSection *targetResolveSection = &resolveSections[sectionHeaders[targetIndex].type];
		char *targetData = (char*)targetResolveSection->getBlob(sectionDomainIds[targetIndex]);
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

		ResolveSection *targetResolveSection = &resolveSections[targetTy];
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

		ResolveSection *targetResolveSection = &resolveSections[targetTy];
		uint32_t targetDomainId = targetResolveSection->getDomainId(targetSectionId);
		void *targetData = nullptr;
		if (targetDomainId != ~0)
			targetData = targetResolveSection->getWrapped(targetDomainId);
		memcpy(patch, &targetData, 4);	
	}
}

void hackResolveReceiver(char *data, size_t size, ResolveSection *resolveSections) {
	DRMHeader header;
	memcpy(&header, data, sizeof(header));
	std::vector<DRMSectionHeader> sectionHeaders(header.sectionCount);
	memcpy(sectionHeaders.data(), data + sizeof(header), sizeof(DRMSectionHeader) * header.sectionCount);
}

}
