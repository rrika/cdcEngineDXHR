#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "DRM.h"

// retain info about DRM structure for displaying in imgui

class DRMIndex {
public:
	std::unordered_map<std::string, std::vector<cdc::DRMSectionHeader>> sectionHeaders;
};
