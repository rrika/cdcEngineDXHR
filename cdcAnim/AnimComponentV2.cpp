#include "AnimComponentV2.h"
#include "cdcAnim/AnimFragment.h" // for cdc::BoneSet
#include "cdcAnim/AnimGraphFactory.h"
#include "cdcAnim/AnimNodes/AnimFragmentNode.h"
#include "cdcAnim/AnimPoseNode.h"
#include "cdcAnim/IAnimGraphNode.h" // for cdc::AnimContextData
#include "cdcKit/Animation/anitracker.h"
#include "cdcMath/MathUtil.h"
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

static bool AlmostZero(Vector& v) {
	float limit = 1.f/0x4000;
	if (-limit < v.x && v.x < limit)
		if (-limit < v.y && v.y < limit)
			if (-limit < v.z && v.z < limit)
				return true;
	return false;
}

void AnimComponentV2::BuildSegTransformForRoot(Matrix& a, Matrix& b) {
	AnimBuffer *buffer = poseNode->pose.buffer;
	AnimSegment *segments = buffer ? buffer->segments : nullptr;
	if (!segments || AlmostZero(segments[0].trans) && AlmostZero(segments[0].rot)) {
		a = b;

	} else {
		Quat rot { segments[0].rot };
		MathUtil::QuatLogToMatrix(&a, &rot);
		a.m[3][0] = segments[0].trans.x;
		a.m[3][1] = segments[0].trans.y;
		a.m[3][2] = segments[0].trans.z;
		a = b * a;
		b = a;
	}
	//
}

void AnimComponentV2::BuildSegTransforms() {
	Segment *modelSegments = model->GetSegmentList();
	uint32_t parent0 = modelSegments[0].parent;
	BuildSegTransformForRoot(matrices[0], matrices[parent0]);
}


}
