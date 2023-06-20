#pragma once

namespace dtp { struct Model; }

class Instance;

namespace cdc {

class IAnimGraphNode;

class AnimComponentV2 {
public:
	dtp::Model *model = nullptr; // 20
	Instance *instance; // 24
	IAnimGraphNode *firstNode = nullptr; // 54

	AnimComponentV2(Instance *instance);
	void Init(dtp::Model *model);
};

}
