#include <cstring> // for memcpy
#include "GenericSection.h"

namespace cdc {

uint32_t GenericSection::realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	alreadyLoaded = false;
	return (uint32_t) new char[size];
}

void GenericSection::fill(uint32_t id, void* src, uint32_t size, uint32_t offset) {
	char *buffer = (char*)id;
	memcpy(buffer + offset, src, size);
}

void *GenericSection::getWrapped(uint32_t id) {
	return (void*)id;
}

uint32_t GenericSection::getDomainId(uint32_t id) {
	return id;
}

}
