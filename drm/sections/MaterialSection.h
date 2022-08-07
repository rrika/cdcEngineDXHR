#pragma once
#include "../ResolveSection.h"
#include <unordered_map>

namespace cdc {

class IMaterial;

// this is the only resolvesection that cares about unknown06 from the section header
// however, at least scripts and dtp have non-zero values in unk06 as well

class MaterialSection : public ResolveSection {
	struct MapEntry {
		IMaterial *material;
		char *blob;
		uint32_t refCount;
		uint32_t size;
		uint32_t unknown6;
	};
	std::unordered_map<uint32_t, MapEntry> materials;
public:
	uint32_t realize(uint32_t sectionId, uint32_t unknown, uint32_t size, bool& alreadyLoaded) override;
	// inherit allocate
	void fill(uint32_t id, void* src, uint32_t size, uint32_t offset) override;
	void construct(uint32_t, void *) override;
	void* getWrapped(uint32_t) override;
	void* getBlob(uint32_t) override;
	uint32_t getDomainId(uint32_t) override;
	~MaterialSection() = default;
};

}
