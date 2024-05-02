#include <cstring> // for memcpy
#include "DTPDataSection.h"

DTPDataSection::DTPData *DTPDataSection::dtpData[0x18000];

uint32_t DTPDataSection::StartResource(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	if (sectionId >= 0x18000) {
		// FatalError("Out of range DTPData ID [%d >= %d]!", sectionId, 0x18000);
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

void DTPDataSection::HandleResourceData(uint32_t id, void* src, uint32_t size, uint32_t offset) {
	memcpy(dtpData[id]->data + offset, src, size);
}

void DTPDataSection::HandleResourceEnd(uint32_t id, void *drmSectionHeader) {
	// TODO
}

void DTPDataSection::construct(uint32_t id, void *drmSectionHeader) {
	dtpData[id]->awaitingConstruction = false;
	// TODO
}

void* DTPDataSection::GetBasePointer(uint32_t id) {
	return (void*)dtpData[id]->data;
}

uint32_t DTPDataSection::FindResource(uint32_t id) {
	if (dtpData[id] && dtpData[id]->refCount > 0)
		return id;
	return ~0u;
}

void *DTPDataSection::getPointer(uint32_t id) {
	if (id >= 0x18000)
		return nullptr;

	DTPData *dtp = dtpData[id];
	if (!dtp || dtp->awaitingConstruction)
		return nullptr;

	return (void*)dtp->data;
}
