#pragma once
#include <string>
#include <unordered_map>
#include "cdcAnim/AnimData.h"
#include "cdc/dtp/animationpipelinegraph.h"
#include "cdc/dtp/animationstategraph.h"

namespace cdc {

class AnimComponentV2;
class AnimStateGraph;

class AnimGraphFactory {
public:
	using AnimNodeFactoryMethod = void(*)(PlugInfo*, AnimComponentV2*, const char*, void*);

	void CreateStateGraph(PlugInfo *retVal, AnimComponentV2 *ac, dtp::AnimStateGraph*, bool withContents);
	void CreateStateGraphContents(PlugInfo *retVal, AnimComponentV2 *ac, dtp::AnimStateGraph*);
	void CreatePipe(PlugInfo *retVal, AnimComponentV2 *ac, char* debugName, dtp::AnimGraphNode*);
	void CreatePipeNode(PlugInfo *retVal, AnimComponentV2 *ac, dtp::AnimGraphNode::Node*);
	void CreateEmpty(PlugInfo *retVal, AnimComponentV2 *ac, void*);
	void CreateRegisteredNode(PlugInfo *retVal, AnimComponentV2 *ac, char *debugNodeName, uint32_t hashedName, void *data);
	void CreateTransientState(PlugInfo *retVal, AnimComponentV2 *ac, AnimStateGraph*);
	void RegisterFactoryMethod(const char *name, AnimNodeFactoryMethod create);

private:
	std::unordered_map<std::string, AnimNodeFactoryMethod> mFactoryMap;
};

}