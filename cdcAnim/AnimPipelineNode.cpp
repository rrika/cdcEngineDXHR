#include <cfloat>
#include "AnimPipelineNode.h"

namespace cdc {

AnimPipelineNode::AnimPipelineNode(AnimComponentV2 *animComponent, /*uint32_t,*/ uint32_t numInputs, bool addToChain) :
	IAnimGraphNode(animComponent, addToChain)
{
	inputs.resize(numInputs);
}

void AnimPipelineNode::Activate(Instance *instance, float f) {
	for (auto input : inputs)
		if (input)
			input->Activate(instance, f);
}

float AnimPipelineNode::GetNextSyncPoint(AnimSyncInfoData *sync) {
	float earliest = FLT_MAX;
	for (auto input : inputs)
		if (input) {
			float t = input->GetNextSyncPoint(sync);
			if (t < earliest)
				earliest = t;
		}

	return earliest;
}

void AnimPipelineNode::GetSyncInfo(AnimSyncInfoData *sync, float *elapsedOut, float *durationOut) {
	*elapsedOut = 0.0f;
	*durationOut = 0.0f;
}

void AnimPipelineNode::Deactivate(Instance *instance) {
	for (auto input : inputs)
		if (input)
			input->Deactivate(instance);
}

void AnimPipelineNode::Update(AnimUpdateData *data) {
	for (auto input : inputs)
		if (input)
			input->Update(data);
}

void AnimPipelineNode::PrePhysics(AnimContextData *args) {
	for (auto input : inputs)
		if (input)
			input->PrePhysics(args);
}

}
