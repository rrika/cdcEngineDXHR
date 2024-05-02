#pragma once
#include "ResolveSection.h"

namespace cdc {

class GenericSection : public ResolveSection {
public:
	uint32_t StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x04
	// inherit StartResource
	void HandleResourceData(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	// inherit construct
	void *GetBasePointer(uint32_t) override; // 0x30
	// inherit GetResolveBasePointer
	uint32_t FindResource(uint32_t) override; // 0x44
};

}
