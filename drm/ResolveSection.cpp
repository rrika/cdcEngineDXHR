#include "ResolveSection.h"

namespace cdc {

ResolveSection::~ResolveSection() {}

uint32_t ResolveSection::realize(
	uint32_t sectionId,
	uint32_t unknown,
	uint32_t size, 
	bool& alreadyLoaded)
{
	return -1;
}

uint32_t ResolveSection::allocate(
	uint32_t sectionId,
	uint32_t flags,
	uint32_t unknown,
	uint32_t size,
	bool& alreadyLoaded)
{
	return realize(sectionId, unknown, size, alreadyLoaded);
}

void *ResolveSection::getBlob(uint32_t sectionId) {
	return getWrapped(sectionId);
}

}
