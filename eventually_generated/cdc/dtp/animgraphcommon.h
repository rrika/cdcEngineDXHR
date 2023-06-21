#pragma once
#include <cstdint>

namespace dtp {

struct AnimStateGraph;

struct AnimGraphExt { // guessed name
	uint16_t mode;
	uint16_t animId;
};

struct AnimGraphRootRef {
	AnimStateGraph *graph;
	AnimGraphExt *ext;
};

}