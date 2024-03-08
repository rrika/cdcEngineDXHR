#include "AnimComponentV2.h"
#include "cdcAnim/AnimFragment.h"
#include "cdcAnim/IAnimGraphNode.h" // for cdc::AnimContextData
#include "cdcMath/MathUtil.h"
#include "cdcWorld/cdcWorldTypes.h" // for dtp::Model*

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
}

void AnimComponentV2::BuildTransforms() {
	// TODO
	pose.AllocSegs(model->oldNumSegments, instance, 1);
	pose.ClearSegValues(1.0);
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
		data.pose = &pose; // HACK
		graphOutput->PrePhysics(&data); // HACK, should be called on PoseNode
	}
}

void AnimComponentV2::BuildSegTransformForRoot(Matrix& a, Matrix& b) {
	AnimBuffer *buffer = pose.buffer;
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

	AnimBuffer *buffer = pose.buffer;
	AnimSegment *animSegments = buffer ? buffer->segments : nullptr;
	uint32_t numSegments = model->oldNumSegments;
	for (int i=1; i<numSegments; i++) {
		MathUtil::QuatLogToMatrix(&matrices[i], (Quat const*)&animSegments[i].rot);
		matrices[i].m[3][0] = animSegments[i].trans.x;
		matrices[i].m[3][1] = animSegments[i].trans.y;
		matrices[i].m[3][2] = animSegments[i].trans.z;
		matrices[i] = matrices[modelSegments[i].parent] * matrices[i];
	}
}

}
