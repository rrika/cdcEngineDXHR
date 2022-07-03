#pragma once
#include <cstdint>

namespace cdc {

class ResolveObject;

class ResolveSection {
public:
	virtual ~ResolveSection() = 0;
	virtual uint32_t realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded); // 0x04
	virtual uint32_t allocate(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded); // 0x08
	virtual void fill(uint32_t id, void* src, uint32_t size, uint32_t offset) = 0; // 0x18
	virtual void construct(uint32_t id, void *drmSectionHeaderMaybe) {}; // 0x24
	virtual void* getWrapped(uint32_t) = 0;     // 0x30
	virtual void* getBlob(uint32_t);            // 0x34
	virtual uint32_t getDomainId(uint32_t) = 0; // 0x44
};

extern cdc::ResolveSection *g_resolveSections[16];

}
