#include "AnimGraphFactory.h"

namespace cdc {

AnimGraphFactory AnimGraphFactory::s_factory;

void AnimGraphFactory::CreateStateGraph(PlugInfo *retVal, AnimComponentV2 *ac, dtp::AnimStateGraph*, bool withContents) {
	// TODO
}

void AnimGraphFactory::CreateStateGraphContents(PlugInfo *retVal, AnimComponentV2 *ac, dtp::AnimStateGraph*) {
	// TODO
}

void AnimGraphFactory::CreatePipe(PlugInfo *retVal, AnimComponentV2 *ac, char* debugName, dtp::AnimGraphNode*) {
	// TODO
}

void AnimGraphFactory::CreatePipeNode(PlugInfo *retVal, AnimComponentV2 *ac, dtp::AnimGraphNode::Node*) {
	// TODO
}

void AnimGraphFactory::CreateEmpty(PlugInfo *retVal, AnimComponentV2 *ac, void*) {
	// TODO
}

void AnimGraphFactory::CreateRegisteredNode(PlugInfo *retVal, AnimComponentV2 *ac, const char *name, void *data) {
	PlugInfo p;
	mFactoryMap[name](&p, ac, name, data);
	*retVal = std::move(p);
}

void AnimGraphFactory::CreateTransientState(PlugInfo *retVal, AnimComponentV2 *ac, AnimStateGraph*) {
	// TODO
}

void AnimGraphFactory::RegisterFactoryMethod(const char *name, AnimNodeFactoryMethod create) {
	// TODO
}



}