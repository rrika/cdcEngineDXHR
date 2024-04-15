#include "config.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcWorld/Object.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/InstanceManager.h"
#include "cdcWorld/InstncG2.h"
#include "cdc/dtp/objectbasedata.h"
#include "UberObject.h"

#if ENABLE_IMGUI
#include "../imgui/imgui.h"
#endif

Instance *UBEROBJECT_BirthSectionInstance(Instance *parent, uint32_t modelIndex, uint32_t id) {
	// TODO
	Instance *instance = InstanceManager::CreateInstance2();
	instance->DefaultInit(
		parent->object,
		modelIndex,
		id,
		parent->intro,
		parent->introData,
		&parent->position,
		&parent->rotation,
		/*pDerivedObject=*/ nullptr,
		/*flags=*/ 0);

	instance->position = parent->position; // but DefaultInit just did that
	instance->rotation = parent->rotation; // but DefaultInit just did that

	// TODO

	instance->objectComponent.SetInstance(instance);

	// TODO

	dtp::ObjectBaseData *dtpData = instance->object->dtpData;
	if (dtpData->hasAnimGraph || dtpData->numHostedAnimGraphs) {
		dtp::Model *model = instance->GetModels()[modelIndex];
		instance->animComponentV2 = new cdc::AnimComponentV2(instance);
		instance->animComponentV2->Init(model);
	}

	return instance;
}

UberObjectComposite::UberObjectComposite(Instance *instance, bool deferSectionInit) {
	auto *prop = (dtp::UberObjectProp*) instance->object->data; // actually reads a cached version thereof
	UserDataComponent::CreateIfNeeded(instance)->userData = this;
	// TODO
	this->instance = instance;
	// TODO

	((cdc::InstanceDrawable*)instance->instanceDrawable)->EnableNoDraw();

	numSections = prop->numSections;
	sectionList = new int32_t[numSections];
	for (uint32_t i=0; i<numSections; i++)
		sectionList[i] = 0x7FFFFFFF;

	if (!deferSectionInit)
		CreateSections(instance);
}

uint32_t CurrentBirthID = 0x1000000; // how is this initialized

void UberObjectComposite::CreateSections(Instance *instance) {
	if (!createdSections) {
		createdSections = true;
		auto *prop = (dtp::UberObjectProp*) instance->object->data; // actually reads a cached version thereof
		if (sectionsIntroUniqueID == ~0u) {
			sectionsIntroUniqueID = CurrentBirthID;
			CurrentBirthID += prop->numSections;
		}

		for (uint32_t i=0; i<numSections; i++) {
			Instance *child = CreateSectionInstance(instance, &prop->sectionList[i], i);
			sectionList[i] = child ? child->introUniqueID : 0x7FFFFFFF;
		}
	}
}

Instance *UberObjectComposite::CreateSectionInstance(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index) {
	Instance *child = UBEROBJECT_BirthSectionInstance(instance, info->modelIndex, sectionsIntroUniqueID + index);
	// TODO
	if (child) {
		// TODO
		cdc::G2Instance_SetTransformsToIdentity(child);
		createSection(child, info, index); // virtual call
	}
	return child;
}

bool UberObjectComposite::checkCondition(dtp::UberObjectProp::QueryProp&) {
	// TODO
	return true;
}

void UberObjectComposite::commandByName(uint32_t key) {
	auto *prop = (dtp::UberObjectProp *)instance->objectData;
	auto cm = prop->commandMapList;
	for (uint32_t i=0; i<prop->numCommandMaps; i++)
		if (cm[i].key == key)
			return commandByIndex(cm[i].commandIndex);
}

void UberObjectComposite::commandByIndex(int index) {
	// check in-range
	auto *prop = (dtp::UberObjectProp *)instance->objectData;
	if (index < 0 || index >= prop->numCommands)
		return;

	// all conditions fulfilled?
	dtp::UberObjectProp::Command *command = &prop->commandList[index];
	for (uint32_t i=0; i<command->numConditions; i++)
		if (auto *cond = command->conditions[i])
			if (checkCondition(*cond) == false)
				return;


	// perform all consequences
	for (uint32_t i=0; i<command->numConsequences; i++) {
		auto *conseq = &command->consequences[i];
		auto target = conseq->targetSection;
		if (target < 0 || target >= numSections)
			continue;
		if (sectionList[target] == 0x7FFFFFFF)
			continue;
		// Instance *targetInstance = Instance::Find(sectionList[target]);
		// if (UserDataComponent *u = targetInstance->userDataComponent)
		// 	if (auto uos = static_cast<UberObjectSection*>(u->userData))
		// 		if (uos->magic == 0xF0012345)
		// 			uos->method4(*conseq);
	}
}

bool UberObjectComposite::methodC(dtp::UberObjectProp::Unknown& entry, int x, void *y) {
	for (uint32_t i=0; i<entry.numConditions; i++)
		if (auto *cond = entry.conditions[i])
			if (checkCondition(*cond) == false)
				return false;
	if (!instance)
		return false;
	// TODO: call NsUberObjectBase::scriptMethod6(UberObjectEvent {objListIndex, x, instanceType, y->dword18});
	return true;
}

UberObjectSection *UberObjectComposite::createSection(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index) {
	return new UberObjectSection(instance, this, info, index);
}

UberObjectSection::UberObjectSection(Instance *instance, UberObjectComposite *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index) {
	// TODO
	sectionProp = info;
	UserDataComponent::CreateIfNeeded(instance)->userData = this;
}

void UberObjectSection::method4(dtp::UberObjectProp::Consequence& conseq) {
	// TODO
	switch (conseq.kind) {
	case 1:
		// TODO
	case 2:
		// TODO
	case 3:
		// TODO
	case 4:
		// TODO
	default:
		break;
	}
}

#if ENABLE_IMGUI
static void buildUI(UIActions& uiact, dtp::UberObjectProp::SectionSub18& sub18) {
	switch (sub18.kind) {
	case 2:
	case 3:
	case 4: {
		uint16_t animId = *(uint16_t*)(6+(uint8_t*)&sub18);
		ImGui::SameLine();
		ImGui::Text("set animation %d", animId);
		break;
	}
	case 54: {
		uint16_t animIndex = *(uint16_t*)(20+(uint8_t*)&sub18);
		ImGui::SameLine();
		ImGui::Text("set audio duration for animation in slot %d", animIndex);
		break;
	}
	case 55: {
		ImGui::SameLine();
		ImGui::Text("send signal %d", sub18.dword4);
		break;
	}
	case 12: {
		ImGui::SameLine();
		ImGui::Text("run command %d", sub18.dword4);
		break;
	}
	case 25: {
		ImGui::SameLine();
		ImGui::Text("fx related %d", sub18.dword4);
		break;
	}
	case 61: {
		ImGui::SameLine();
		ImGui::Text("drawable setup");
		break;
	}
	case 0: {
		ImGui::SameLine();
		ImGui::Text("nothing?");
		break;
	}
	case 28: {
		ImGui::SameLine();
		ImGui::Text("sound related (dtp %x)", sub18.dword4);
		break;
	}
	case 33: {
		ImGui::SameLine();
		ImGui::Text("set visibility? %d", sub18.dword4);
		break;
	}
	case 77: {
		ImGui::SameLine();
		ImGui::Text("transition 40/41 in controlled instance");
		break;
	}
	case 80: {
		ImGui::SameLine();
		ImGui::Text("transition 44 in controlled instance");
		break;
	}
	case 81: {
		ImGui::SameLine();
		ImGui::Text("transition 45 in controlled instance");
		break;
	}
	case 86: {
		ImGui::SameLine();
		ImGui::Text("clear tv queue");
		break;
	}
	case 87: {
		ImGui::SameLine();
		ImGui::Text("default tv queue");
		break;
	}
	default: break;
	}
}

static void buildUI(UIActions& uiact, dtp::UberObjectProp::QueryProp *query) {
	ImGui::Text("Condition");
}

static void buildUI(UIActions& uiact, dtp::UberObjectProp *uberProp, dtp::UberObjectProp::Consequence& conseq) {
	ImGui::Text("consequence for %d:",
		conseq.targetSection);
		ImGui::SameLine();
	switch (conseq.kind) {
	case 1: {
		uint32_t nextState = *(uint32_t*)(&conseq.pad[0]);
		ImGui::Text("go to state %d", nextState);
		break;
	}
	case 2: {
		uint32_t gti = *(uint32_t*)(&conseq.pad[0]);
		dtp::UberObjectProp::SectionProp *sectionProp = &uberProp->sectionList[conseq.targetSection];
		auto& trans = sectionProp->transitions[gti];
		ImGui::Text("trigger transition %d towards %d", gti, trans.dword4);
		break;
	}
	case 3: {
		uint32_t key = *(uint32_t*)(&conseq.pad[0]);
		ImGui::Text("trigger transitions with key 15 %d", key);
		break;
	}
	case 4: {
		uint32_t key = *(uint32_t*)(&conseq.pad[0]);
		ImGui::Text("trigger transitions with key 18 %d", key);
		break;
	}
	case 5: {
		uint32_t flagIndex = *(uint32_t*)(&conseq.pad[0]);
		uint32_t polarity = conseq.pad[4];
		ImGui::Text("%s flag %d", polarity ? "set" : "clear", flagIndex);
		break;
	}
	case 6: {
		auto *sub18 = (dtp::UberObjectProp::SectionSub18*)conseq.pad;
		ImGui::Text("sub18 %d", sub18->kind);
		buildUI(uiact, *sub18);
		break;
	}
	case 7: {
		uint32_t signalIndex = *(uint32_t*)(&conseq.pad[0]);
		ImGui::Text("send signal %d", signalIndex);
		break;
	}
	case 9: {
		ImGui::Text("disturbance (TODO)");
		break;
	}
	default:
		ImGui::Text("unknown type %d", conseq.kind);
		break;
	}
}

void buildUI(UIActions& uiact, dtp::UberObjectProp *uberProp) {
	ImGui::Indent();
	for (uint32_t i=0; i < uberProp->numSections; i++) {
		auto& section = uberProp->sectionList[i];
		ImGui::Text("Section %d: modelIndex=%d initialState=%d", i, section.modelIndex, section.initialState);
		ImGui::Indent();
		for (uint32_t j=0; j < section.numSubs; j++) {
			auto& sub = section.sub[j];
			ImGui::Text("State %d: %x %x %x countC %d count14 %d count1C %d count24 %d count2C %d", j,
				sub.dword0,
				sub.dword4,
				sub.dword8,
				sub.countC,
				sub.count14,
				sub.count1C,
				sub.count24,
				sub.count2C);
			ImGui::Indent();
			for (uint32_t k=0; k < sub.countC; k++) {
				auto& sub10 = *sub.sub10[k];
				ImGui::Text("transition[%d] -> state %d %d %d", k,
					sub10.dword4,
					sub10.dword18,
					sub10.dword1C);
			}
			for (uint32_t k=0; k < sub.count14; k++) {
				ImGui::Text("enter[%d] %d", k, sub.sub18[k].kind);
				buildUI(uiact, sub.sub18[k]);
			}
			for (uint32_t k=0; k < sub.count1C; k++) {
				ImGui::Text("exit[%d] %d", k, sub.sub20[k].kind);
				buildUI(uiact, sub.sub20[k]);
			}
			for (uint32_t k=0; k < sub.count24; k++) {
				ImGui::Text("cond enter[%d] %d", k, sub.sub28[k].rest.kind);
				buildUI(uiact, sub.sub28[k].rest);
			}
			for (uint32_t k=0; k < sub.count2C; k++) {
				ImGui::Text("cond exit[%d] %d", k, sub.sub30[k].rest.kind);
				buildUI(uiact, sub.sub30[k].rest);
			}
			ImGui::Unindent();
		}
		ImGui::Unindent();
	}
	for (uint32_t i=0; i < uberProp->numCommandMaps; i++) {
		auto& cmdmap = uberProp->commandMapList[i];
		ImGui::Text("CommandMap %08x -> %d", cmdmap.key, cmdmap.commandIndex);
	}
	for (uint32_t i=0; i < uberProp->numCommands; i++) {
		auto& cmd = uberProp->commandList[i];
		ImGui::Text("Command %08x", cmd.dword0);
		ImGui::Indent();
		for (uint32_t j=0; j < cmd.numConditions; j++) {
			buildUI(uiact, cmd.conditions[j]);
		}
		for (uint32_t j=0; j < cmd.numConsequences; j++) {
			buildUI(uiact, uberProp, cmd.consequences[j]);
		}
		ImGui::Unindent();
	}
	for (uint32_t i=0; i < uberProp->numUnknown; i++) {
		auto& unknown = uberProp->unknownList[i];
		ImGui::Text("Event for section %d, %d %d",
			unknown.sectionIndex,
			unknown.dword10,
			unknown.dword14);
		ImGui::Indent();
		for (uint32_t j=0; j < unknown.numConditions; j++) {
			buildUI(uiact, unknown.conditions[j]);
		}
		ImGui::Unindent();
	}
	for (uint32_t i=0; i < uberProp->dword28; i++) {
		ImGui::Text("Unknown2");
	}
	for (uint32_t i=0; i < uberProp->numQueries; i++) {
		auto& query = uberProp->queryList[i];
		buildUI(uiact, &query);
	}
	ImGui::Unindent();
}
#endif
