#pragma once
#include "ResolveSection.h"

class DTPDataSection : public cdc::ResolveSection {
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

	static DTPData *dtpData[0x18000];
public:
	// inherit StartResource
	uint32_t StartResource(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x08
	void HandleResourceData(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	void HandleResourceEnd(uint32_t id, void *drmSectionHeader) override; // 0x1C
	void construct(uint32_t id, void *drmSectionHeaderMaybe) override; // 0x24
	void* GetBasePointer(uint32_t) override; // 0x30
	// inherit GetResolveBasePointer
	uint32_t FindResource(uint32_t) override; // 0x44

	static void *getPointer(uint32_t);
};
