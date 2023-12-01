#include <cstring>
#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcAnim/AnimFragment.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "cdc/dtp/animnodes/fragment.h"
#include "cdc/dtp/animationpipelinegraph.h"
#include "cdc/dtp/animationstategraph.h"
#include "cdc/dtp/animgraphcommon.h"
#include "cdc/dtp/objectbasedata.h"

using namespace cdc;

static void buildUI(UIActions& uiact, dtp::AnimGraphNode *pipeline, dtp::AnimGraphExt *ext);
static void buildUI(UIActions& uiact, dtp::AnimGraphNode::Node *node, dtp::AnimGraphExt *ext);
static void buildUI(UIActions& uiact, dtp::AnimStateGraph *stateGraph, dtp::AnimGraphExt *ext);
static void buildUI(UIActions& uiact, dtp::AnimStateGraph::State *state, dtp::AnimGraphExt *ext);

static void buildUI(UIActions& uiact, dtp::AnimGraphNode *pipeline, dtp::AnimGraphExt *ext) {
	ImGui::Text("pipeline %p", pipeline);
	ImGui::Indent();
	for (uint32_t i=0; i<pipeline->numNodes; i++)
		buildUI(uiact, &pipeline->nodes[i], ext);
	ImGui::Unindent();
}

static void buildUI(UIActions& uiact, dtp::AnimGraphNode::Node *node, dtp::AnimGraphExt *ext) {
	const char *types[] = {
		"AnimNode",
		"EmbeddedPipe",
		"EmbeddedState",
		"CustomNode"
	};
	ImGui::Text("node %p %s", node, types[node->type4]);
	if (node->type4 == dtp::AnimGraphNode::NodeTypeEnum_AnimNode ||
		node->type4 == dtp::AnimGraphNode::NodeTypeEnum_CustomNode)
	{
		ImGui::SameLine();
		ImGui::Text("%s", (const char*)node->data);
		ImGui::Indent();
		if (strcmp((const char*)node->data, "cdc\\dtp\\animnodes\\Fragment.dtp") == 0 ||
			strcmp((const char*)node->data, "dtp\\animnodes\\DynamicFragment.dtp") == 0
		) {
			auto *data = (dtp::FragmentNode*)node->namedNodeData;
			int32_t extIndex = (int32_t)data->extIndex;
			if (extIndex != -1) {
				uint16_t animId = ext[extIndex].animId;
				ImGui::Text("ext[%d] = %04x", extIndex, animId);
			} else {
				ImGui::Text("ext[%d] = N/A", extIndex);
			}
		}
		ImGui::Unindent();
	}

	// ImGui::Indent();
	// ImGui::Unindent();
}

static void buildUI(UIActions& uiact, dtp::AnimStateGraph *stateGraph, dtp::AnimGraphExt *ext) {
	ImGui::Text("state graph %p", stateGraph);
	ImGui::Indent();
	for (uint32_t i=0; i<stateGraph->numStates; i++)
		buildUI(uiact, &stateGraph->states[i], ext);
	ImGui::Unindent();
}

static void buildUI(UIActions& uiact, dtp::AnimStateGraph::State *state, dtp::AnimGraphExt *ext) {
	const char *types[] = {
		"AnimPipe",
		"StateGraph",
		"TransientState",
		"Unknown",
		"Fragment",
		"Empty"
	};
	ImGui::Text("state %p %s", state, types[state->type4]);
	ImGui::Indent();
	if (state->type4 == dtp::AnimStateGraph::AnimPipeOrStateGraph_AnimPipe) {
		auto *pipeline = (dtp::AnimGraphNode*) state->data;
		buildUI(uiact, pipeline, ext);
	}

	ImGui::Unindent();
}

void buildUI(UIActions& uiact, cdc::AnimFragment *fragment) {
	ImGui::Text("fragment %p", fragment);
	if (!fragment)
		return;
	ImGui::Indent();
	ImGui::Text("mAnimID 0x%x", fragment->mAnimID);
	ImGui::Text("mKeyCount %d", fragment->mKeyCount);
	ImGui::Text("mTimePerKey %d", fragment->mTimePerKey);
	ImGui::Text("mSegmentCount %d", fragment->mSegmentCount);
	ImGui::Text("mSectionCount %d", fragment->mSectionCount);
	ImGui::Text("mExtraChannelCount %d", fragment->mExtraChannelCount);
	ImGui::Unindent();
}

void buildUI(UIActions& uiact, AnimComponentV2 *ac) {
	dtp::ObjectBaseData *objectDtpData = ac->instance->object->dtpData;
	for (uint32_t i=0; i<objectDtpData->hasAnimGraph; i++) {
		auto& root = objectDtpData->pAnimGraph[i]; 
		buildUI(uiact, root.graph, root.ext);
	}
}
