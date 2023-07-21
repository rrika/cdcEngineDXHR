#pragma once
#include <cstdint>

namespace dtp {
	struct AnimGraphExt;
	struct Model;
}

class Instance;

namespace cdc {

class AnimPoseNode;
class IAnimGraphNode;
struct BoneSet;
struct Matrix;

class AnimComponentV2 {
	static BoneSet s_allBones;
public:
	IAnimGraphNode *graphOutput = nullptr; // 0
	AnimPoseNode *poseNode = nullptr; // 4
	dtp::Model *model = nullptr; // 20
	Instance *instance; // 24
	Matrix *matrices = nullptr; // 2C, assigned by G2Instance_RebuildTransforms
	IAnimGraphNode *firstNode = nullptr; // 54

	AnimComponentV2(Instance *instance);
	void Init(dtp::Model *model);
	AnimPoseNode *CreatePoseNode();
	dtp::AnimGraphExt *getExt(uint32_t index);
	void BuildTransforms();
	void PrePhysics();
	void BuildSegTransforms();
	void BuildSegTransformForRoot(Matrix&, Matrix&);
};

}
