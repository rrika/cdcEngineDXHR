#pragma once
#include <cstdint>

namespace cdc {

class ResolveObject;

class ResolveSection {
public:
	static void Create();

	virtual ~ResolveSection() = 0;
	virtual uint32_t StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded); // 0x04
	virtual uint32_t StartResource(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded); // 0x08
	virtual void HandleResourceData(uint32_t id, void* src, uint32_t size, uint32_t offset) = 0; // 0x18
	virtual void HandleResourceEnd(uint32_t id, void *drmSectionHeader) = 0; // 0x1C
	virtual void construct(uint32_t id, void *drmSectionHeader) {}; // 0x24 (either HandleResourceReady or HandleReceiveCompleted)
	virtual void* GetBasePointer(uint32_t) = 0; // 0x30
	virtual void* GetResolveBasePointer(uint32_t); // 0x34
	virtual uint32_t FindResource(uint32_t) = 0; // 0x44
};

extern cdc::ResolveSection *g_resolveSections[16];

}
