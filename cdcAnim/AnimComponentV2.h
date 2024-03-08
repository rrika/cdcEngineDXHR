#pragma once
#include "cdcAnim/AnimPose.h"

namespace dtp { struct Model; }

class Instance;

namespace cdc {

class IAnimGraphNode;
struct BoneSet;
struct Matrix;

class AnimComponentV2 {
	static BoneSet s_allBones;
public:
	IAnimGraphNode *graphOutput = nullptr; // 0
	AnimPose pose; // HACK
	dtp::Model *model = nullptr; // 20
	Instance *instance; // 24
	Matrix *matrices = nullptr; // 2C, assigned by G2Instance_RebuildTransforms
	IAnimGraphNode *firstNode = nullptr; // 54

	AnimComponentV2(Instance *instance);
	void Init(dtp::Model *model);
	void BuildTransforms();
	void PrePhysics();
	void BuildSegTransformForRoot(Matrix&, Matrix&);
	void BuildSegTransforms();
};

}
