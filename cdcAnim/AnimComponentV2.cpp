#include "AnimComponentV2.h"
#include "cdcAnim/AnimFragment.h" // for cdc::BoneSet
#include "cdcAnim/AnimGraphFactory.h"
#include "cdcAnim/AnimPoseNode.h"
#include "cdcAnim/IAnimGraphNode.h" // for cdc::AnimContextData
#include "cdcWorld/cdcWorldTypes.h" // for dtp::Model*
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "cdc/dtp/animgraphcommon.h"

namespace cdc {

BoneSet AnimComponentV2::s_allBones {1.0, 0, 249};

AnimComponentV2::AnimComponentV2(Instance *instance) :
	instance(instance)
{
	// TODO
}

void AnimComponentV2::instantiate(dtp::Model *model) {
	// TODO
	this->model = model;
}

dtp::AnimGraphExt *AnimComponentV2::getExt(uint32_t index) {
	// TODO
	return &instance->object->dtpData->pAnimGraph->ext[index];
}

void AnimComponentV2::BuildTransforms() {
	// TODO
	PrePhysics();
	// TODO
	BuildSegTransforms();
}

void AnimComponentV2::PrePhysics() {
	if (graphOutput0) { // TODO
		AnimContextData data;
		data.instance = instance;
		data.model = model;
		data.boneSet = &s_allBones;
		data.weight = 1.0f;
		poseNode4->PrePhysics(&data);
	}
}

void AnimComponentV2::BuildSegTransforms() {
	//
}

void AnimComponentV2::BuildSegTransformForRoot() {
	//
}

}
