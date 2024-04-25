#pragma once
#include "cdcAnim/AnimPose.h"

namespace dtp { struct Model; }

class Instance;

namespace cdc {

class IAnimGraphNode;
struct Matrix;

class AnimComponentV2 {
public:
	AnimPose pose; // HACK
	dtp::Model *model = nullptr; // 20
	Instance *instance; // 24
	Matrix *matrices = nullptr; // 2C, assigned by G2Instance_RebuildTransforms
	IAnimGraphNode *firstNode = nullptr; // 54

	AnimComponentV2(Instance *instance);
	void Init(dtp::Model *model);
	void BuildTransforms();
	void BuildSegTransformForRoot(Matrix&, Matrix&);
	void BuildSegTransforms();
};

}
