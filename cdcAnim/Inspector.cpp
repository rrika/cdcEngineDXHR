#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "cdc/dtp/animationpipelinegraph.h"
#include "cdc/dtp/animationstategraph.h"
#include "cdc/dtp/objectbasedata.h"

using namespace cdc;

static void buildUI(UIActions& uiact, dtp::AnimGraphNode *pipeline);
static void buildUI(UIActions& uiact, dtp::AnimGraphNode::Node *node);
static void buildUI(UIActions& uiact, dtp::AnimStateGraph *stateGraph);
static void buildUI(UIActions& uiact, dtp::AnimStateGraph::State *state);

static void buildUI(UIActions& uiact, dtp::AnimGraphNode *pipeline) {
	ImGui::Text("pipeline %p", pipeline);
	ImGui::Indent();
	for (uint32_t i=0; i<pipeline->numNodes; i++)
		buildUI(uiact, &pipeline->nodes[i]);
	ImGui::Unindent();
}

static void buildUI(UIActions& uiact, dtp::AnimGraphNode::Node *node) {
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
	}
	// ImGui::Indent();
	// ImGui::Unindent();
}

static void buildUI(UIActions& uiact, dtp::AnimStateGraph *stateGraph) {
	ImGui::Text("state graph %p", stateGraph);
	ImGui::Indent();
	for (uint32_t i=0; i<stateGraph->numStates; i++)
		buildUI(uiact, &stateGraph->states[i]);
	ImGui::Unindent();
}

static void buildUI(UIActions& uiact, dtp::AnimStateGraph::State *state) {
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
		buildUI(uiact, pipeline);
	}

	ImGui::Unindent();
}

void buildUI(UIActions& uiact, AnimComponentV2 *ac) {
	dtp::ObjectBaseData *objectDtpData = ac->instance->object->dtpData;
	for (uint32_t i=0; i<objectDtpData->dword48; i++)
		buildUI(uiact, objectDtpData->pAnimGraphReq4C[i]);
}
