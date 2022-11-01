#include <cfloat>
#include "AnimPipelineNode.h"

namespace cdc {

AnimPipelineNode::AnimPipelineNode(AnimComponent *animComponent, /*uint32_t,*/ uint32_t numInputs, bool addToChain) :
	IAnimGraphNode(animComponent, addToChain)
{
	inputs.resize(numInputs);
}

void AnimPipelineNode::Activate(Instance *instance, float f) {
	for (auto input : inputs)
		if (input)
			input->Activate(instance, f);
}

float AnimPipelineNode::GetNextSyncPoint(void *args) {
	float earliest = FLT_MAX;
	for (auto input : inputs)
		if (input) {
			float t = input->GetNextSyncPoint(args);
			if (t < earliest)
				earliest = t;
		}

	return earliest;
}

void AnimPipelineNode::GetSyncInfo(void*, float *elapsedOut, float *durationOut) {
	*elapsedOut = 0.0f;
	*durationOut = 0.0f;
}

void AnimPipelineNode::Deactivate(Instance *instance) {
	for (auto input : inputs)
		if (input)
			input->Deactivate(instance);
}

void AnimPipelineNode::Update(void *a) {
	for (auto input : inputs)
		if (input)
			input->Update(a);
}

void AnimPipelineNode::PrePhysics(AnimGraphNodeMethod18Args *args) {
	for (auto input : inputs)
		if (input)
			input->PrePhysics(args);
}

}
