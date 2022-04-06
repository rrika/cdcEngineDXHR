#pragma once
#include "../ResolveSection.h"

namespace cdc {

class RenderResourceSection : public ResolveSection {
public:
	uint32_t realize(uint32_t sectionId, uint32_t unknown, uint32_t size, uint8_t& success) override;
	uint32_t allocate(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, uint8_t& success) override;
	void fill(uint32_t id, void* src, size_t size, size_t offset) override;
	void* getWrapped(uint32_t) override;
	void* getBlob(uint32_t) override;
	uintptr_t getDomainId(uint32_t) override;
	~RenderResourceSection() = default;
};

}
