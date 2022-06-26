#include <cstring> // for memcpy
#include "DTPDataSection.h"

namespace cdc {

uint32_t DTPDataSection::allocate(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	if (sectionId >= 0x18000) {
		// cdcError("Out of range DTPData ID [%d >= %d]!", sectionId, 0x18000);
		return ~0u;
	}

	DTPData *&dtp = dtpData[sectionId];

	if (dtp) {
		dtp->refCount++;
		alreadyLoaded = true;
		return sectionId;

	} else {
		// dtp = (DTPData*) new (sectionAllocator, 0) char[size+16];
		dtp = (DTPData*) new char[size+16];
		dtp->sectionWord6 = unknown6;
		dtp->refCount = 1;
		dtp->awaitingConstruction = true;
		dtp->resourceType = sectionSubType;
		alreadyLoaded = false;
		return sectionId;
	}
}

void DTPDataSection::fill(uint32_t id, void* src, uint32_t size, uint32_t offset) {
	memcpy(dtpData[id]->data + offset, src, size);
}

void DTPDataSection::construct(uint32_t id, void *drmSectionHeaderMaybe) {
	dtpData[id]->awaitingConstruction = false;
	// TODO
}

void* DTPDataSection::getWrapped(uint32_t id) {
	return (void*)dtpData[id]->data;
}

uint32_t DTPDataSection::getDomainId(uint32_t id) {
	if (dtpData[id] && dtpData[id]->refCount > 0)
		return id;
	return ~0u;
}

}
