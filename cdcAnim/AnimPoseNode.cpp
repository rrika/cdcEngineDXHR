#include <cfloat>
#include "AnimPoseNode.h"

namespace cdc {

AnimPoseNode::AnimPoseNode(AnimComponentV2 *ac, bool addToChain) :
	IAnimGraphNode(ac, addToChain)
{
	// TODO
}

void AnimPoseNode::Activate(Instance *instance, float f) {
	if (inputNode)
		inputNode->Activate(instance, f);
}

void AnimPoseNode::Deactivate(Instance *instance) {
	if (inputNode)
		inputNode->Deactivate(instance);
}

float AnimPoseNode::GetNextSyncPoint(AnimSyncInfoData *sync) {
	if (inputNode)
		return inputNode->GetNextSyncPoint(sync);
	else
		return FLT_MAX;
}

void AnimPoseNode::GetSyncInfo(AnimSyncInfoData *sync, float *elapsedOut, float *durationOut) {
	if (inputNode)
		inputNode->GetSyncInfo(sync, elapsedOut, durationOut);
}

void AnimPoseNode::Update(AnimUpdateData *data){
	if (inputNode)
		inputNode->Update(data);
}

void AnimPoseNode::PrePhysics(AnimContextData *data) {

	const bool useBlend = false;

	if (inputNode) {
		if (data->weight < 0.00001)
			return;
		AnimContextData childData = *data;
		// childData.instance = data->instance;
		// childData.model    = data->model;
		// childData.boneSet  = data->boneSet;
		// childData.weight   = data->weight;
		childData.pose = &pose;
		pose.ClearSegValues(1.0f);
		inputNode->PrePhysics(&childData);
	}
	if (useBlend && data->pose) {
		// TODO
	}
}

IAnimGraphNode *AnimPoseNode::GetOutput(uint32_t index) {
	return index == 0 ? outputNode : nullptr;
}

uint32_t AnimPoseNode::GetNumOutputs() {
	return outputNode ? 1 : 0;
}

void AnimPoseNode::SetOutput(uint32_t index, IAnimGraphNode *node) {
	if (index == 0)
		outputNode = node;
}

IAnimGraphNode *AnimPoseNode::GetInput(uint32_t index) {
	return index == 0 ? inputNode : nullptr;
}

uint32_t AnimPoseNode::GetNumInputs() {
	return inputNode ? 1 : 0;
}

void AnimPoseNode::SetInput(uint32_t index, IAnimGraphNode *node) {
	if (index == 0)
		inputNode = node;
}

}