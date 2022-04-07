#include "RenderResourceSection.h"
#include "../../rendering/CommonRenderDevice.h"
#include "../../rendering/RenderResource.h"

namespace cdc {

uint32_t RenderResourceSection::realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& success) {
	return ~0;
}

uint32_t RenderResourceSection::allocate(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	auto &entry = resources[sectionId];

	if (entry.refCount != 0) {
		entry.refCount++;
		alreadyLoaded = true;
		return sectionId;

	} else {
		entry.resource = CommonRenderDevice::createResource(sectionSubType, size);
		entry.size = size;
		entry.refCount = 1;
		entry.todoLoading = true;
		alreadyLoaded = false;
		return sectionId;
	}
}

void RenderResourceSection::fill(uint32_t sectionId, void* src, size_t size, size_t offset) {
	RenderResource *res = resources[sectionId].resource;
	if (sectionId == 0x1F6E)
		return; // huh, interesting

	res->resFill(src, size, offset);
}

void* RenderResourceSection::getWrapped(uint32_t sectionId) {
	if (auto it = resources.find(sectionId); it != resources.end())
		return (void*)it->second.resource;
	return nullptr;
}

void* RenderResourceSection::getBlob(uint32_t sectionId) {
	RenderResource *res = resources[sectionId].resource;
	return (void*)res->resGetBuffer();
}

uint32_t RenderResourceSection::getDomainId(uint32_t sectionId) {
	if (auto it = resources.find(sectionId); it != resources.end())
		return sectionId;
	else
		return ~0u;
}

}
