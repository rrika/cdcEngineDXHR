#include "ShaderLibSection.h"
#include "cdcRender/CommonRenderDevice.h"
#include "cdcRender/shaders/IShaderLib.h"

namespace cdc {

uint32_t ShaderLibSection::StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	auto &entry = shaders[sectionId];

	if (entry.refCount != 0) {
		entry.refCount++;
		alreadyLoaded = true;
		return sectionId;

	} else {
		entry.shaderlib = g_renderDevice->createShaderLib(size);
		entry.size = size;
		entry.refCount = 1;
		alreadyLoaded = false;
		return sectionId;
	}
}

void ShaderLibSection::ReleaseResource(uint32_t id) {
	auto &entry = shaders[id];
	if (--entry.refCount == 0) {
		entry.shaderlib->Release();
		shaders.erase(id);
	}
}

void ShaderLibSection::HandleResourceData(uint32_t sectionId, void* src, uint32_t size, uint32_t offset) {
	auto &entry = shaders[sectionId];
	bool done = offset + size == entry.size;
	entry.shaderlib->fill((char*)src, offset, size, done);
}

void ShaderLibSection::HandleResourceEnd(uint32_t id, void *drmSectionHeader) {
	// TODO
}

void* ShaderLibSection::GetBasePointer(uint32_t sectionId) {
	if (auto it = shaders.find(sectionId); it != shaders.end())
		return it->second.shaderlib;
	return nullptr;
}

uint32_t ShaderLibSection::FindResource(uint32_t sectionId) {
	if (auto it = shaders.find(sectionId); it != shaders.end())
		return sectionId;
	else
		return ~0u;
}

}
