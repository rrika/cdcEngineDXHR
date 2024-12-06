#include <cstdint>
#include <cstdio>
#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcAnim/AnimDecoder.h"
#include "cdcAnim/AnimFragment.h"
#include "cdcAnim/AnimPoseNode.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "UIActions.h"
#include "cdc/dtp/animnodes/fragment.h"
#include "cdc/dtp/animationpipelinegraph.h"
#include "cdc/dtp/animationstategraph.h"
#include "cdc/dtp/animgraphcommon.h"

using namespace cdc;

static void buildUI(UIActions& uiact, dtp::AnimGraphNode *pipeline, dtp::AnimGraphExt *ext);
static void buildUI(UIActions& uiact, dtp::AnimGraphNode::Node *node, dtp::AnimGraphExt *ext);
static void buildUI(UIActions& uiact, dtp::AnimStateGraph *stateGraph, dtp::AnimGraphExt *ext);
static void buildUI(UIActions& uiact, dtp::AnimStateGraph::State *state, dtp::AnimGraphExt *ext);

static void buildUI(UIActions& uiact, dtp::AnimGraphNode *pipeline, dtp::AnimGraphExt *ext) {
	ImGui::Text("pipeline %p", pipeline);
	uiact.origin((void*)pipeline);
	ImGui::Indent();
	for (uint32_t i=0; i<pipeline->numNodes; i++)
		buildUI(uiact, &pipeline->nodes[i], ext);
	for (uint32_t i=0; i<pipeline->numEdges; i++) {
		auto& edge = pipeline->edges[i];
		ImGui::Text("edge %d:%d -> %d:%d",
			edge.srcNodeIndex,
			edge.srcNodeOutputIndex,
			edge.dstNodeIndex,
			edge.dstNodeInputIndex);
		uiact.origin((void*)&edge);
	}
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
		uiact.origin((void*)node);
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
			for (uint32_t i=0; i<data->m_loopTriggerCount; i++) {
				auto& event = data->m_loopTriggers[i];
				ImGui::Text("loop trigger %08x %08x", event.dword0, event.dword4);
			}
			for (uint32_t i=0; i<data->m_endTriggerCount; i++) {
				auto& event = data->m_endTriggers[i];
				ImGui::Text("end trigger %08x %08x", event.dword0, event.dword4);
			}
		}
		ImGui::Unindent();
	}

	// ImGui::Indent();
	// ImGui::Unindent();
}

static void buildUI(UIActions& uiact, dtp::AnimStateGraph *stateGraph, dtp::AnimGraphExt *ext) {
	ImGui::Text("state graph %p nodes", stateGraph);
	uiact.origin((void*)stateGraph);
	ImGui::Indent();
	for (uint32_t i=0; i<stateGraph->numStates; i++)
		buildUI(uiact, &stateGraph->states[i], ext);
	ImGui::Unindent();
	ImGui::Text("edges");
	ImGui::Indent();
	for (uint32_t i=0; i<stateGraph->numEdges; i++) {
		auto& edge = stateGraph->edges[i];
		ImGui::Text("%d -> %d", edge.fromIndex, edge.toIndex);
		uiact.origin((void*)&edge);
	}
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

static float cbShortArray(void* data, int idx) {
	auto *offsets = (int16_t*)data;
	return offsets[idx] / 4096.f;
}

static float cbLinear16(void* data, int idx) {
	auto *decoder = (AnimDecoder*)data;
	return decoder->GetValues(true, idx);
}

static float cbLinear32(void* data, int idx) {
	auto *decoder = (AnimDecoder*)data;
	return decoder->GetValues(false, idx);
}

void buildUI(UIActions& uiact, cdc::AnimFragment *fragment) {
	ImGui::Text("fragment %p", fragment);
	if (!fragment)
		return;
	uiact.origin((void*)fragment);
	ImGui::Indent();
	ImGui::Text("mAnimID 0x%x", fragment->mAnimID);
	ImGui::Text("mKeyCount %d", fragment->mKeyCount);
	ImGui::Text("mTimePerKey %d", fragment->mTimePerKey);
	ImGui::Text("mSegmentCount %d", fragment->mSegmentCount);
	ImGui::Text("mSectionCount %d", fragment->mSectionCount);
	ImGui::Text("mExtraChannelCount %d", fragment->mExtraChannelCount);

	ImGui::Indent();
	if (fragment->mKeyCount == 1) { // decode pose
		uint16_t *masks = fragment->mSegKeyListPtr + 1;
		float *values = fragment->mValueDataPtr + 1;
		for (int i=0; i < fragment->mSegmentCount; i++) {
			ImGui::Text("bone %d", i);
			uint8_t mask = uint8_t(*masks++);
			for (int j=0; j<8; j++) {
				if ((mask >> j) & 1) {
					ImGui::SameLine();
					ImGui::Text("%c=%f", "ijklxyzw"[j], *values++);
				}
			}
		}
	} else { // decode frame
		uint16_t *masks = fragment->mSegKeyListPtr + 1;
		uint16_t *lengths = fragment->mLengthDataPtr;
		char *values = (char*)(fragment->mValueDataPtr + 1);
		for (int i=0; i < fragment->mSegmentCount; i++) {
			uint8_t mask = uint8_t(*masks++);
			ImGui::Text("bone %d mask=%x", i, mask);
			for (int j=0; j<8; j++) {
				if ((mask >> j) & 1) {
					uint16_t mode = lengths[0];
					static const char *modes[] = { "tabulated", "constant", "piece-wise linear", "zero" };
					ImGui::Text(" channel %c %s @ %p %p",
						"ijklxyzw"[j],
						mode < 4 ? modes[mode] : "invalid mode",
						(void*)values,
						(void*)lengths);
					if (mode == 0) { // tabulated
						uint16_t fmt16 = lengths[1];
						if (fmt16) {
							auto *offsets = (int16_t*)values;
							//for (int k=0; k<fragment->mKeyCount; k++)
							//	ImGui::Text("  [%d] = %d", k, offsets[k]);
							ImGui::PlotLines("Lines", &cbShortArray, (void*)offsets, fragment->mKeyCount); // , 0, 0, -1.0f, 1.0f, ImVec2(0, 80.0f));
							values += sizeof(uint16_t) * ((fragment->mKeyCount+1) & ~1);
						} else {
							auto *offsets = (float*)values;
							//for (int k=0; k<fragment->mKeyCount; k++)
							//	ImGui::Text("  [%d] = %f", k, offsets[k]);
							ImGui::PlotLines("Lines", offsets, fragment->mKeyCount); // , 0, 0, -1.0f, 1.0f, ImVec2(0, 80.0f));
							values += sizeof(float) * fragment->mKeyCount;
						}
						lengths += 2;

					} else if (mode == 1) { // constant
						ImGui::Text("  %f", *(float*)values);
						values += sizeof(float);
						lengths++;

					} else if (mode == 2) { // piece-wise linear
						const char *fmts[] = { "f32", "i16" };
						uint16_t fmt = lengths[1];
						uint16_t numEntries = lengths[2];
						ImGui::Text("  %s #points %d", fmts[fmt], numEntries);
						AnimDecoder decoder;
						decoder.SetChannel(fmt, values, (uint8_t*)lengths);
						// ImGui::Text("  sum of lengths %d", decoder.mSumOfLengths);
						if (fmt) {
							ImGui::PlotLines("Lines", &cbLinear16, (void*)&decoder, fragment->mKeyCount);
						} else {
							ImGui::PlotLines("Lines", &cbLinear32, (void*)&decoder, fragment->mKeyCount);
						}
						if (fmt) {
							values += sizeof(uint16_t) * ((numEntries+1) & ~1);
						} else {
							values += sizeof(float) * numEntries;
						}
						lengths = (uint16_t*)(uintptr_t(lengths) + 5 + (numEntries|1));
					} else { // zero
						// empty
					}
				}
			}
		}
	}

	uint16_t *extraInfo = fragment->mExtraChannelLengthPtr;
	float *extraValues = fragment->mExtraChannelDataPtr;
	for (int i=0; i < fragment->mExtraChannelCount; i++) {
		uint8_t vectorIndex = *extraInfo++;
		uint8_t componentIndex = *extraInfo++;
		ImGui::Text("extra[%d].%c=%f", vectorIndex, "?xyzw"[componentIndex], *extraValues++);
	}
	ImGui::Unindent();

	ImGui::Unindent();
}

void buildUI(UIActions& uiact, AnimComponentV2 *ac) {
	auto *object = ac->instance->object;
	dtp::ObjectBaseData *objectDtpData = object->dtpData;
	for (uint32_t i=0; i<objectDtpData->hasAnimGraph; i++) {
		auto& root = objectDtpData->pAnimGraph[i]; 
		buildUI(uiact, root.graph, root.ext);
	}
	for (uint32_t i=0; i<object->numAnims; i++) {
		char label[10];
		snprintf(label, 10, "Anim %d", i);
		if (ImGui::SmallButton(label))
			ac->TriggerStateTransition(0, i, true);
	}
	if (ac->poseNode) {
		char label[7];
		cdc::AnimBuffer &buffer = *ac->poseNode->pose.buffer;
		for (uint32_t i=1; i<buffer.numSegments; i++) {
			snprintf(label, 7, "pos%d", i);
			ImGui::SliderFloat3(label, buffer.segments[i].trans.vec128, -200.f, 200.f);
			snprintf(label, 7, "rot%d", i);
			ImGui::SliderFloat3(label, buffer.segments[i].rot.vec128, -2.f, 2.f);
		}
	}
}
