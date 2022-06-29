#include <cstring> // for memcpy
#include "MaterialSection.h"
#include "../../rendering/CommonRenderDevice.h"
#include "../../rendering/IMaterial.h"

namespace cdc {

uint32_t MaterialSection::realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	auto &entry = materials[sectionId];

	if (entry.refCount != 0) {
		entry.refCount++;
		alreadyLoaded = true;
		return sectionId;

	} else {
		entry.material = g_renderDevice->createMaterial();
		entry.blob = new char[size]; // TODO
		entry.refCount = 1;
		entry.size = size;
		entry.unknown6 = unknown6;
		alreadyLoaded = false;
		return sectionId;
	}
}

void MaterialSection::fill(uint32_t sectionId, void* src, size_t size, size_t offset) {
	auto &entry = materials[sectionId];
	memcpy(entry.blob + offset, src, size);
}

void MaterialSection::construct(uint32_t sectionId, void *) {
	auto &entry = materials[sectionId];
	entry.material->load((MaterialBlob*)entry.blob);
	entry.blob = nullptr;
}

void* MaterialSection::getWrapped(uint32_t sectionId) {
	return materials[sectionId].material;
}

void* MaterialSection::getBlob(uint32_t sectionId) {
	return materials[sectionId].blob;
}

uint32_t MaterialSection::getDomainId(uint32_t sectionId) {
	if (auto it = materials.find(sectionId); it != materials.end())
		return sectionId;
	else
		return ~0u;
}

}
