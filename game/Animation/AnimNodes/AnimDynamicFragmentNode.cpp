#include "AnimDynamicFragmentNode.h"
#include "cdcAnim/AnimGraphFactory.h"

using namespace cdc;

AnimGraphFactory::Registry<AnimDynamicFragmentNode, DynamicFragmentNode>
	s_nodeFactory_AnimDynamicFragmentNode("dtp\\animnodes\\DynamicFragment.dtp");

AnimDynamicFragmentNode::AnimDynamicFragmentNode(AnimComponentV2 *ac, DynamicFragmentNode *data) :
	AnimFragmentNode(ac, &data->fragment)
{
	// TODO
}
