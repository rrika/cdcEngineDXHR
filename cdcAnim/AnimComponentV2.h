#pragma once
#include <cstdint>

namespace dtp {
	struct AnimGraphExt;
	struct Model;
}

class Instance;

namespace cdc {

class IAnimGraphNode;

class AnimComponentV2 {
public:
	dtp::Model *model = nullptr; // 20
	Instance *instance; // 24
	IAnimGraphNode *firstNode = nullptr; // 54

	AnimComponentV2(Instance *instance);
	void instantiate(dtp::Model *model);
	dtp::AnimGraphExt *getExt(uint32_t index);
};

}
