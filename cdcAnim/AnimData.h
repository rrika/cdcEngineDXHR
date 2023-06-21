#pragma once
#include <cstdint>
#include <vector>

namespace cdc {

class IAnimGraphNode;

struct Plug {
	IAnimGraphNode *node;
	uint32_t index;
};

struct PlugInfo {
	std::vector<Plug> outputPlugs;
	std::vector<Plug> inputPlugs;
};

}
