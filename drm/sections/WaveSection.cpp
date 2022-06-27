#include "WaveSection.h"

namespace cdc {

WaveSectionEntry *WaveSection::lookupEntry(
	uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded)
{
	// TODO
	return &entries[sectionId];
}

uint32_t WaveSection::realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	auto *entry = lookupEntry(sectionId, unknown6, size, alreadyLoaded);
	if (!alreadyLoaded && entry->refCount > 1)
		alreadyLoaded = true;

	return sectionId;
}

void WaveSection::fill(uint32_t sectionId, void *src, size_t size, size_t offset) {
	// TODO
}

void *WaveSection::getWrapped(uint32_t sectionId) {
	return nullptr;
}

uint32_t WaveSection::getDomainId(uint32_t sectionId) {
	if (entries[sectionId].refCount != 0)
		return sectionId;
	else
		return ~0u;
}

}
