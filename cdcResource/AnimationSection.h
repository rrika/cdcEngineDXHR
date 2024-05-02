#pragma once
#include "ResolveSection.h"

namespace cdc {

class AnimationSection : public ResolveSection {
public:
	uint32_t StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override;
	void HandleResourceData(uint32_t id, void *src, uint32_t size, uint32_t offset) override;
	void HandleResourceEnd(uint32_t id, void *drmSectionHeader) override {}
	void* GetBasePointer(uint32_t) override;
	uint32_t FindResource(uint32_t) override;
};

}
