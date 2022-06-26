#pragma once
#include "../ResolveSection.h"

namespace cdc {

class DTPDataSection : public ResolveSection {
	struct DTPData {
		uint16_t refCount;
		uint16_t sectionWord6;
		uint8_t awaitingConstruction;
		uint8_t resourceType;
		uint16_t word6;
		uint32_t dword8;
		uint32_t dwordC;
		char data[];
	};

	DTPData *dtpData[0x18000];
public:
	// inherit realize
	uint32_t allocate(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x08
	void fill(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	void construct(uint32_t id, void *drmSectionHeaderMaybe) override; // 0x24
	void* getWrapped(uint32_t) override; // 0x30
	// inherit getBlob
	uint32_t getDomainId(uint32_t) override; // 0x44
};

}
