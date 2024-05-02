#pragma once
#include "ResolveSection.h"
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
	uint32_t StartResource(uint32_t sectionId, uint32_t unknown, uint32_t size, bool& alreadyLoaded) override;
	// inherit StartResource
	void HandleResourceData(uint32_t id, void* src, uint32_t size, uint32_t offset) override;
	void* GetBasePointer(uint32_t) override;
	// inherit GetResolveBasePointer
	uint32_t FindResource(uint32_t) override;
	~ShaderLibSection() = default;
};

}
