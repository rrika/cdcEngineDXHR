#pragma once
#include <map>
#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "DRM.h"

// retain info about DRM structure for displaying in imgui

struct LoadedSection {
	std::shared_ptr<std::string> drmPath;
	uint32_t sectionIndex;
	uint32_t size; // 0xffffffff indicates wrapped resource
};

class DRMIndex {
public:
	std::unordered_map<std::string, std::vector<cdc::DRMSectionHeader>> sectionHeaders;
	std::map<uintptr_t, LoadedSection> loadedSections;
};

extern DRMIndex drmIndex;

bool locate(void *ptr, cdc::DRMSectionHeader *&header, ptrdiff &offset);
