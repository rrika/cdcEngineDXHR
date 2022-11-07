#pragma once
#include "ResolveSection.h"

namespace cdc {

class ObjectSection : public ResolveSection {
public:
	uint32_t realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x04
	void fill(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	void construct(uint32_t id, void *drmSectionHeaderMaybe) override; // 0x24
	void* getWrapped(uint32_t) override; // 0x30
	void* getBlob(uint32_t) override; // 0x34
	uint32_t getDomainId(uint32_t) override;  // 0x44
};

}
