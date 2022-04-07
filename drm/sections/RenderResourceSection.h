#pragma once
#include <unordered_map> // not sure what STL this game uses
#include "../ResolveSection.h"

namespace cdc {

class RenderResource;

class RenderResourceSection : public ResolveSection {
	struct MapEntry {
		RenderResource *resource = nullptr;
		uint32_t refCount = 0;
		uint32_t size = 0;
		uint8_t todoLoading = 0;
	};
	std::unordered_map<uint32_t, MapEntry> resources;
public:
	uint32_t realize(uint32_t sectionId, uint32_t unknown, uint32_t size, bool& alreadyLoaded) override;
	uint32_t allocate(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override;
	void fill(uint32_t id, void* src, size_t size, size_t offset) override;
	void* getWrapped(uint32_t) override;
	void* getBlob(uint32_t) override;
	uint32_t getDomainId(uint32_t) override;
	~RenderResourceSection() = default;
};

}
