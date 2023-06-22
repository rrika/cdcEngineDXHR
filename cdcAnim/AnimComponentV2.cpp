#include "AnimComponentV2.h"
#include "cdcAnim/AnimFragment.h" // for cdc::BoneSet
#include "cdcAnim/AnimGraphFactory.h"
#include "cdcAnim/AnimNodes/AnimFragmentNode.h"
#include "cdcAnim/AnimPoseNode.h"
#include "cdcAnim/IAnimGraphNode.h" // for cdc::AnimContextData
#include "cdcKit/Animation/anitracker.h"
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

void AnimComponentV2::Init(dtp::Model *model) {
	// TODO
	this->model = model;

	if (true) { // HACK
		Object *object = instance->object;
		AnimFragment *fragment = nullptr;
		if (object->numAnims > 0) {
			uint32_t animID = object->animations[0].animID;
			int32_t i = ANITRACKER_FindAnimation(animID);
			if (i != -1)
				fragment = (AnimFragment*)aniTracker[i].animationData;
		}

		if (fragment) {
			graphOutput = new AnimFragmentNode(this, fragment);
			poseNode = CreatePoseNode();
			poseNode->SetInput(0, graphOutput);
		}
	}
}

AnimPoseNode *AnimComponentV2::CreatePoseNode() {
	return new AnimPoseNode(this, true);
}

dtp::AnimGraphExt *AnimComponentV2::getExt(uint32_t index) {
	// TODO
	return &instance->object->dtpData->pAnimGraph->ext[index];
}

void AnimComponentV2::BuildTransforms() {
	poseNode->pose.AllocSegs(model->oldNumSegments, instance, 1);
	poseNode->pose.ClearSegValues(1.0);
	PrePhysics();
	// TODO
	BuildSegTransforms();
}

void AnimComponentV2::PrePhysics() {
	if (graphOutput) { // TODO
		AnimContextData data;
		data.instance = instance;
		data.model = model;
		data.boneSet = &s_allBones;
		data.weight = 1.0f;
		poseNode->PrePhysics(&data);
	}
}

void AnimComponentV2::BuildSegTransforms() {
	//
}

void AnimComponentV2::BuildSegTransformForRoot() {
	//
}

}
