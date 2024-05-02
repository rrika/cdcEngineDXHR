#include <cstdio>
#include "RenderResourceSection.h"
#include "../../rendering/CommonRenderDevice.h"
#include "../../rendering/RenderResource.h"

namespace cdc {

uint32_t RenderResourceSection::StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	return ~0;
}

uint32_t RenderResourceSection::StartResource(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
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

void RenderResourceSection::HandleResourceData(uint32_t sectionId, void* src, uint32_t size, uint32_t offset) {
	RenderResource *res = resources[sectionId].resource;
	if (sectionId == 0x1F6E)
		return; // huh, interesting

	if (res == nullptr) // HACK
		printf("TODO: res is nullptr in RenderResourceSection::HandleResourceData for sectionId %x\n", sectionId);
	else
		res->resFill(src, size, offset);
}

void RenderResourceSection::construct(uint32_t id, void *) {
	MapEntry& entry = resources[id];
	if (id == 0x1F6E)
		// TODO
		return;

	// printf("  resConstruct %p\n", entry.resource);

	if (entry.resource) // HACK
		entry.resource->resConstruct();
	entry.todoLoading = false;
}


void* RenderResourceSection::GetBasePointer(uint32_t sectionId) {
	if (auto it = resources.find(sectionId); it != resources.end())
		return (void*)it->second.resource;
	return nullptr;
}

void* RenderResourceSection::GetResolveBasePointer(uint32_t sectionId) {
	RenderResource *res = resources[sectionId].resource;

	if (!res) // HACK
		return nullptr;

	return (void*)res->resGetBuffer();
}

uint32_t RenderResourceSection::FindResource(uint32_t sectionId) {
	if (auto it = resources.find(sectionId); it != resources.end())
		return sectionId;
	else
		return ~0u;
}

}
