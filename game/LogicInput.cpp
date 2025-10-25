#include "cdcGameShell/cdcGameShell.h"
#include "cdcResource/DTPDataSection.h"
#include "cdcResource/ResolveObject.h"
#include "cdcFile/FileHelpers.h"
#include "cdcFile/FileSystem.h"
#include "LogicInput.h"
#include "input/Input.h"
#include "UIActions.h"
#include "config.h"

using namespace cdc;

static ResolveObject *s_logicInputResolveObject = nullptr;
static LogicInput *s_logicInputs = nullptr;
static uint32_t s_numLogicInputs = 0;

void logicinput_load_database() {
	void *logicinputDatabase = nullptr;
	char path[256] = "\0";
	GameShell::LOAD_ObjectFileName(path, "logicinput_database");
	s_logicInputResolveObject = ResolveObject::create(
		path, nullptr, nullptr, nullptr,
		&logicinputDatabase, nullptr, nullptr, 0, cdc::FileRequest::NORMAL);

	FileSystem *fs = getDefaultFileSystem();
	fs->processAll();

	// [count]
	// [dtp id 0]
	// [dtp id 1]
	// ...

	s_numLogicInputs = *(uint32_t*)logicinputDatabase;
	s_logicInputs = new LogicInput[s_numLogicInputs];

	if (logicinputDatabase) {
		uint32_t *dtpIds = 1+(uint32_t*)logicinputDatabase;
		for (uint32_t i=0; i<s_numLogicInputs; i++) {
			s_logicInputs[i].fromDTP(dtpIds[i]);
		}
	}
}

LogicInput *logicinput_from_dtp_index(uint32_t dtpId) {
	for (uint32_t i=0; i<s_numLogicInputs; i++)
		if (s_logicInputs[i].m_dtpIndex == dtpId)
			return &s_logicInputs[i];
	return nullptr;
}

void LogicInput::fromDTP(uint32_t dtpId) {
	m_dtpData = (LogicInputBlob*) DTPDataSection::getPointer(dtpId);
	m_dtpIndex = dtpId;
	for (uint32_t i=0; i<4; i++) {
		m_time[i] = 0.0f;
		m_byte30[i] = 0;
		m_sequenceChildIndex[i] = -1;
		m_cachedSequenceActivation[i] = 0;
	}
	m_returnOneOnce = false;
	m_useFixedValue = 0;
	m_fixedValue = 0;
}

void LogicInput::resetSequences(uint32_t playerIndexMaybe) {
	resetSequences(&m_dtpData->mainNode, playerIndexMaybe);
}

void LogicInput::resetSequences(LogicInputNode *node, uint32_t playerIndexMaybe) {
	if (auto *gate = node->asGateNode()) { // 2
		for (uint32_t i=0; i < gate->count; i++)
			resetSequences(&gate->nodes[i], playerIndexMaybe);

	} else if (auto *seq = node->asSequenceNode()) { // 3
		m_cachedSequenceActivation[playerIndexMaybe] = 0;
		m_sequenceChildIndex[playerIndexMaybe] = 0;
	}
}

int32_t LogicInput::evalNodeInt(LogicInputNode *node, uint32_t inputSystemIndex) {
	if (m_useFixedValue)
		return m_fixedValue;

	if (m_returnOneOnce) {
		m_returnOneOnce = false;
		return 1;
	}

	if (auto *button = node->asButtonNode()) { // 0, 5
		// TODO

	} else if (auto *axis = node->asAxisNode()) { // 1
		// TODO

	} else if (auto *gate = node->asGateNode()) { // 2
		// TODO

	} else if (auto *seq = node->asSequenceNode()) { // 3
		return m_cachedSequenceActivation[inputSystemIndex];

	} else if (auto *ind = node->asIndirectNode()) { // 4
		if (!ind->dtpIndex)
			return 0;
		LogicInput *refInput = logicinput_from_dtp_index(ind->dtpIndex);
		refInput->m_fixedValue = 0;
		refInput->m_fixedValue = refInput->evalNodeInt(&refInput->m_dtpData->mainNode, inputSystemIndex);
		return refInput->m_fixedValue;
	}
	return 0;
}

float LogicInput::evalNodeFloat(LogicInputNode *node, bool queryFloatDirectly, uint32_t inputSystemIndex) {
	if (auto *button = node->asButtonNode()) { // 0, 5
		if (evalNodeInt(node, inputSystemIndex))
			return button->analogueValue;
		return 0.0f;

	} else if (auto *axis = node->asAxisNode()) { // 1
		auto axisIndex = 0; // TODO
		Deadzone *deadzone = axis->hasCustomDeadzone ? (Deadzone*)(axis+1) : nullptr;
		return g_inputSystems[inputSystemIndex]->GetNormalizedValue((EInput)axisIndex, deadzone);

	} else if (auto *gate = node->asGateNode()) { // 2
		float sum = 0.0f;
		for (uint32_t i=0; i<gate->count; i++)
			sum += evalNodeFloat(&gate->nodes[i], queryFloatDirectly, inputSystemIndex);
		return sum;

	} else if (auto *seq = node->asSequenceNode()) { // 3
		float sum = evalNodeFloat(&seq->node, queryFloatDirectly, inputSystemIndex);
		for (uint32_t i=0; i<seq->childCount; i++)
			sum += evalNodeFloat(&seq->nodesExt[i].node, queryFloatDirectly, inputSystemIndex);
		return sum;

	} else if (auto *ind = node->asIndirectNode()) { // 4
		if (!ind->dtpIndex)
			return 0.0f;
		LogicInput *refInput = logicinput_from_dtp_index(ind->dtpIndex);

		if (queryFloatDirectly)
			return refInput->evalFloatInternal(inputSystemIndex);
		
		// query int first, then when non-zero query float
		refInput->m_fixedValue = 0;
		refInput->m_fixedValue = refInput->evalNodeInt(&refInput->m_dtpData->mainNode, inputSystemIndex);
		if (refInput->m_fixedValue != 0)
			return evalNodeFloat(&refInput->m_dtpData->mainNode, false, inputSystemIndex);
		return 0.0f;
	}
	__builtin_unreachable();
}

float LogicInput::evalFloatInternal(uint32_t inputSystemIndex) {
	return evalNodeFloat(&m_dtpData->mainNode, /*queryFloatDirectly=*/true, inputSystemIndex);
}


int32_t LogicInput::evalInt(uint32_t inputSystemIndex, bool needByte30Activation) {
	m_fixedValue = 0;
	if (needByte30Activation && !m_byte30[inputSystemIndex])
		return 0;

	return m_fixedValue = evalNodeInt(&m_dtpData->mainNode, inputSystemIndex);
}

float LogicInput::evalFloat(uint32_t inputSystemIndex, bool gatedByInt) {
	if (gatedByInt && !evalInt(inputSystemIndex, /*needByte30Activation=*/true))
		return 0.0;

	return evalNodeFloat(&m_dtpData->mainNode, /*queryFloatDirectly=*/false, inputSystemIndex);
}

void LogicInput::forceActive(uint32_t unused) {
	(void) unused;
	m_returnOneOnce = true;
}

uint32_t LogicInput::getDUPR(uint32_t inputSystemIndex) {
	return m_dtpData->mainNode.getDUPR(inputSystemIndex);
}

void LogicInput::timeThresholds(LogicInputNode *node, uint32_t inputSystemIndex) {
	if (auto *gate = node->asGateNode()) { // 2
		// recurse
		for (uint32_t i=0; i<gate->count; i++)
			timeThresholds(&gate->nodes[i], inputSystemIndex);

	} else if (auto *seq = node->asSequenceNode()) { // 3
		// TODO
	}
}

uint32_t LogicInputNode::getDUPR(uint32_t inputSystemIndex) {
	if (auto *button = asButtonNode()) { // 0, 5
		uint32_t keyIndex = getKeyIndex(this);
		if (keyIndex == -1)
			return 0;

		auto *system = g_inputSystems[inputSystemIndex];
		return system->GetDUPR((EInput)keyIndex);

	} else if (auto *axis = asAxisNode()) { // 1
		return 0;

	} else if (auto *gate = asGateNode()) { // 2
		if (gate->count == 0)
			return 0;

		uint32_t numDown = 0, numUp = 0, numPressed = 0, numReleased = 0;
		for (uint32_t i=0; i<gate->count; i++) {
			uint32_t subDUPR = gate->nodes[i].getDUPR(inputSystemIndex);
			if (subDUPR & 2) numDown++;
			if (subDUPR & 4) numUp++;
			if (subDUPR & 8) numPressed++;
			if (subDUPR & 16) numReleased++;
		}

		if (gate->any) {
			if (numDown > 0) {
				return numDown == numPressed ? (8 | 2) : 2;
			} else {
				return numReleased > 0 ? (16 | 4) : 4;
			}
		} else {
			if (numUp > 0) {
				return numUp == numReleased ? (16 | 4) : 4;
			} else {
				return numPressed > 0 ? (8 | 2) : 2;
			}
		}

	} else if (auto *seq = asSequenceNode()) { // 3
		// return DUPR from first node that doesn't return zero
		if (uint32_t v = seq->node.getDUPR(inputSystemIndex))
			return v;

		for (uint32_t i=0; i < seq->childCount; i++)
			if (uint32_t v = seq->nodesExt[i].node.getDUPR(inputSystemIndex))
				return v;

		return 0;

	} else if (auto *ind = asIndirectNode()) { // 4
		if (ind->dtpIndex)
			return logicinput_from_dtp_index(ind->dtpIndex)->getDUPR(inputSystemIndex);
		return 0;
	}
	__builtin_unreachable();
}

uint32_t LogicInputNode::getKeyIndex(LogicInputNode *node) {
	return -1; // TODO
}

#if ENABLE_IMGUI

#include "imgui/imgui.h"

static void buildUI(UIActions& uiact, LogicInputNode *node, LogicInputNodeExt *ext, uint32_t indent) {
	const char *keyModes[] = {"press", "release", "down", "up"};
	const char *analogueModes[] = {"entering", "leaving", "inside", "outside"};
	const char *allEither[] = {"all", "either"};
	const char *tapHold[] = {"activate within", "hold for"};
	auto *system = g_inputSystems[0];

	if (auto *button = node->asButtonNode()) { // 0, 5
		auto *nd = (LogicInputNode0or5*) node->dword4;
		LogicInputNode0or5Key *key = node->type == 0
			? &nd->key0
			: &nd->key5;
		ImGui::Text("%*s%s button %d [%d]", indent, "", keyModes[nd->mode], key->keyindex,
			system->IsKeyDown((EInput)key->keyindex) ? 1 : 0);
		uiact.origin(node);

	} else if (auto *axis = node->asAxisNode()) { // 1
		auto *nd = (LogicInputNode1*) node->dword4;
		auto axisIndex = nd->axis + 66; // 16 -> 82
		ImGui::Text("%*s%s range %f..%f on axis %d (%f)", indent, "",
			analogueModes[nd->mode], nd->min, nd->max, axisIndex,
			system->GetValue((EInput)axisIndex));
		uiact.origin(node);

	} else if (auto *gate = node->asGateNode()) { // 2
		auto *nd = (LogicInputNode2*) node->dword4;
		ImGui::Text("%*s%s of", indent, "", allEither[nd->any]);
		uiact.origin(node);
		if (ext) {
			ImGui::SameLine();
			ImGui::Text("(%s %1.2f sec)", tapHold[ext->byte5], ext->timeThreshold);
		}
		for (uint32_t i=0; i<nd->count; i++)
			buildUI(uiact, &nd->nodes[i], nullptr, indent+2);
		return;

	} else if (auto *seq = node->asSequenceNode()) { // 3
		ImGui::Text("%*ssequence of", indent, "");
		uiact.origin(node);

		if (ext) {
			ImGui::SameLine();
			ImGui::Text("(%s %1.2f sec)", tapHold[ext->byte5], ext->timeThreshold);
		}
		buildUI(uiact, &seq->node, nullptr, indent+2);
		for (uint32_t i=0; i<seq->childCount; i++)
			buildUI(uiact, &seq->nodesExt[i].node, &seq->nodesExt[i], indent+2);
		return;

	} else if (auto *ind = node->asIndirectNode()) { // 4
		auto *blob = (LogicInputBlob*) DTPDataSection::getPointer(ind->dtpIndex);
		const char *name = blob ? blob->name : "(null)";
		ImGui::Text("%*suse logicinput %s (%04x)", indent, "", name, ind->dtpIndex);
		uiact.origin(node);
	}

	if (ext) {
		ImGui::SameLine();
		ImGui::Text("(%s %1.2f sec)", tapHold[ext->byte5], ext->timeThreshold);
	}
}

void logicinput_build_ui(UIActions& uiact) {
	ImGui::Text("num logic inputs = %d", s_numLogicInputs);
	for (uint32_t i=0; i<s_numLogicInputs; i++) {
		LogicInputBlob *blob = s_logicInputs[i].m_dtpData;
		const char *name = blob ? blob->name : "(null)";
		ImGui::Text("%s", name);
		uiact.origin(blob);
		buildUI(uiact, &blob->mainNode, nullptr, 2);
	}
}

#endif
