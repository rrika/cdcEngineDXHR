#pragma once
#include <string>
#include <map>
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
	void CreateRegisteredNode(PlugInfo *retVal, AnimComponentV2 *ac, const char *name, void *data);
	void CreateTransientState(PlugInfo *retVal, AnimComponentV2 *ac, AnimStateGraph*);
	void RegisterFactoryMethod(const char *name, AnimNodeFactoryMethod create);

	static AnimGraphFactory s_factory;

	template <typename CdcNode, typename DtpNode>
	static void DTPCreate(PlugInfo *plugInfo, AnimComponentV2 *ac, const char *name, void *data) {
		auto dtpData = (DtpNode*)(1+(uint32_t*)data);
		IAnimGraphNode *node = new CdcNode(ac, dtpData);
	}

	template <typename CdcNode, typename DtpNode>
	struct Registry {
		Registry(const char *name) {
			s_factory.RegisterFactoryMethod(name, &DTPCreate<CdcNode, DtpNode>);
		}
	};

private:
	std::map<std::string, AnimNodeFactoryMethod> mFactoryMap;
};

}