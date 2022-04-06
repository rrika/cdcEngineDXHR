#include "RenderResourceSection.h"

namespace cdc {

uint32_t RenderResourceSection::realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, uint8_t& success) {
	return ~0;
}

uint32_t RenderResourceSection::allocate(uint32_t sectionId, uint32_t sectionSubType, uint32_t unknown6, uint32_t size, uint8_t& success) {
	// TODO
	return 0;
}

void RenderResourceSection::fill(uint32_t id, void* src, size_t size, size_t offset) {
	// TODO
}

void* RenderResourceSection::getWrapped(uint32_t) {
	// TODO
	return nullptr;
}

void* RenderResourceSection::getBlob(uint32_t) {
	// TODO
	return nullptr;
}

uintptr_t RenderResourceSection::getDomainId(uint32_t sectionId) {
	// TODO: return -1 if nothing found
	return sectionId;
}

}
