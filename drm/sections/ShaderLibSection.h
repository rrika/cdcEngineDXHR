#pragma once
#include "../ResolveSection.h"
#include <unordered_map>

namespace cdc {

class IShaderLib;

class ShaderLibSection : public ResolveSection {
	struct MapEntry {
		IShaderLib *shaderlib;
		uint32_t refCount;
		uint32_t size;
	};
	std::unordered_map<uint32_t, MapEntry> shaders;
public:
	uint32_t realize(uint32_t sectionId, uint32_t unknown, uint32_t size, bool& alreadyLoaded) override;
	// inherit allocate
	void fill(uint32_t id, void* src, uint32_t size, uint32_t offset) override;
	void* getWrapped(uint32_t) override;
	// inherit getBlob
	uint32_t getDomainId(uint32_t) override;
	~ShaderLibSection() = default;
};

}
