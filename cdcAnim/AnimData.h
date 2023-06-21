#pragma once
#include <cstdint>

namespace cdc {

class IAnimGraphNode;

template <typename T>
struct AnimArrayView {
	T *m_pData;
	uint32_t m_size;
};

struct Plug {
	IAnimGraphNode *node;
	uint32_t index;
};

struct PlugInfo {
	AnimArrayView<Plug> outputPlugs;
	AnimArrayView<Plug> inputPlugs;
};

}
