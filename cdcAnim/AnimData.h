#pragma once
#include <cstdint>

namespace cdc {

class AnimComponentV2;
class IAnimGraphNode;

struct AnimUpdateData { // line 223
	Instance *instance; // 0
	float dt; // 4
	void *ptr8; // pAbsTrans maybe
	float floatC; // sync maybe
	float weight; // 10
	uint8_t updateFlags; // 14
	// 3 bytes padding
};

struct AnimSyncInfoData { // line 242
	AnimComponentV2 *ac;
	float advance;
};

}
