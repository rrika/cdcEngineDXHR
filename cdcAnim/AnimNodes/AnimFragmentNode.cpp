#include "AnimFragmentNode.h"
#include "cdcAnim/AnimGraphFactory.h"

namespace cdc {

AnimGraphFactory::Registry<AnimFragmentNode, dtp::FragmentNode>
	s_nodeFactory_AnimDynamicFragmentNode("cdc\\dtp\\animnodes\\Fragment.dtp");

AnimFragmentNode::AnimFragmentNode(AnimComponentV2 *ac, dtp::FragmentNode *data) :
	AnimPipelineNode(ac, /*numInputs=*/ 0, /*addToChain=*/ true)
{
	// TODO
}

void AnimFragmentNode::SetAnimData(uint16_t anim) {
	// TODO
}

void AnimFragmentNode::Activate(Instance*, float) {
	// TODO
}

void AnimFragmentNode::Deactivate(Instance*) {
	// TODO
}

float AnimFragmentNode::GetNextSyncPoint(void*) {
	// TODO
	return 0.0f;
}

void AnimFragmentNode::GetSyncInfo(void*, float *elapsedOut, float *durationOut) {
	// TODO
}

void AnimFragmentNode::Update(void*) {
	// TODO
}

void AnimFragmentNode::PrePhysics(AnimGraphNodeMethod18Args *) {
	// TODO
}

}
