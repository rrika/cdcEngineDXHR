#pragma once
#include "ResolveSection.h"

namespace cdc {

class ObjectSection : public ResolveSection {
public:
	uint32_t StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x04
	void HandleResourceData(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	void HandleResourceEnd(uint32_t id, void *drmSectionHeader) override {}
	void construct(uint32_t id, void *drmSectionHeaderMaybe) override; // 0x24
	void* GetBasePointer(uint32_t) override; // 0x30
	void* GetResolveBasePointer(uint32_t) override; // 0x34
	uint32_t FindResource(uint32_t) override;  // 0x44
};

}
