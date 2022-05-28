#include "ShaderLibSection.h"
#include "../../rendering/CommonRenderDevice.h"
#include "../../rendering/shaders/IShaderLib.h"

namespace cdc {

uint32_t ShaderLibSection::realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	auto &entry = shaders[sectionId];

	if (entry.refCount != 0) {
		entry.refCount++;
		alreadyLoaded = true;
		return sectionId;

	} else {
		entry.shaderlib = gRenderDevice->createShaderLib(size);
		entry.size = size;
		entry.refCount = 1;
		alreadyLoaded = false;
		return sectionId;
	}
}

void ShaderLibSection::fill(uint32_t sectionId, void* src, size_t size, size_t offset) {
	auto &entry = shaders[sectionId];
	bool done = offset + size == entry.size;
	entry.shaderlib->fill((char*)src, offset, size, done);
}

void* ShaderLibSection::getWrapped(uint32_t sectionId) {
	if (auto it = shaders.find(sectionId); it != shaders.end())
		return it->second.shaderlib;
	return nullptr;
}

uint32_t ShaderLibSection::getDomainId(uint32_t sectionId) {
	if (auto it = shaders.find(sectionId); it != shaders.end())
		return sectionId;
	else
		return ~0u;
}

}
