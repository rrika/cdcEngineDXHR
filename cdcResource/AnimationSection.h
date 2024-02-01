#pragma once
#include "ResolveSection.h"

namespace cdc {

class AnimationSection : public ResolveSection {
public:
	uint32_t realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override;
	void fill(uint32_t id, void *src, uint32_t size, uint32_t offset) override;
	void* getWrapped(uint32_t) override;
	uint32_t getDomainId(uint32_t) override;
};

}
