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

AnimFragmentNode::AnimFragmentNode(AnimComponentV2 *ac, AnimFragment *fragment) :
	AnimPipelineNode(ac, /*numInputs=*/ 0, /*addToChain=*/ true),
	fragment(fragment)
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

void AnimFragmentNode::PrePhysics(AnimContextData *data) {
	if (data->weight >= 0.00001) {
		if (data->weight != 0.0) {
			if (fragment) { // AnimFragment
				if (fragment->mKeyCount == 1)
					DecompressPose(data);
				else
					DecompressFrame(data);
			} else {
				// TODO
			}
		}
	}
}

void AnimFragmentNode::DecompressPose(AnimContextData *data) {
	// TODO
}

void AnimFragmentNode::DecompressFrame(AnimContextData *data) {
	// TODO
}

}
