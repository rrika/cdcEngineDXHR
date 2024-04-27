#include "AnimComponentV2.h"
#include "cdcAnim/AnimFragment.h"
#include "cdcAnim/AnimNodes/AnimFragmentNode.h"
#include "cdcAnim/AnimPoseNode.h"
#include "cdcAnim/IAnimGraphNode.h" // for cdc::AnimContextData
#include "cdcKit/Animation/anitracker.h"
#include "cdcMath/MathUtil.h"
#include "cdcMath/VectorInlines.h"
#include "cdcWorld/cdcWorldTypes.h" // for dtp::Model*
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"

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

void AnimComponentV2::BuildTransforms() {
	if (!poseNode)
		return;

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

void AnimComponentV2::TriggerStateTransition(uint32_t trigger, uint32_t hackAnimIndex, bool hackLoop) {
	static_cast<AnimFragmentNode*>(graphOutput)->SetAnimData(hackAnimIndex, hackLoop);
}

void AnimComponentV2::BuildSegTransformForRoot(Matrix& a, Matrix& b) {
	AnimBuffer *buffer = poseNode->pose.buffer;
	AnimSegment *segments = buffer ? buffer->segments : nullptr;
	if (!segments || segments[0].trans.IsZero3() && segments[0].rot.IsZero3()) {
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
	int32_t parent0 = int32_t(modelSegments[0].parent); // index -1 is valid
	BuildSegTransformForRoot(matrices[0], matrices[parent0]);

	AnimBuffer *buffer = poseNode->pose.buffer;
	AnimSegment *animSegments = buffer ? buffer->segments : nullptr;
	uint32_t numSegments = model->oldNumSegments;
	uint32_t numSegments2 = model->GetNumSegments();
	for (int i=1; i<numSegments; i++) {
		MathUtil::QuatLogToMatrix(&matrices[i], (Quat const*)&animSegments[i].rot);
		matrices[i].m[3][0] = animSegments[i].trans.x;
		matrices[i].m[3][1] = animSegments[i].trans.y;
		matrices[i].m[3][2] = animSegments[i].trans.z;
		matrices[i] = matrices[modelSegments[i].parent] * matrices[i];
	}
	// HACK for as long as we don't have a proper cloth sim
	for (int i=numSegments; i<numSegments2; i++) {
		matrices[i] = matrices[1];
	}
}

}
