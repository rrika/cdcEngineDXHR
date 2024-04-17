#include <cstdio>
#include "config.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcSound/SoundPlex.h"
#include "cdcResource/DTPDataSection.h"
#include "cdcWorld/Object.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/InstanceManager.h"
#include "cdcWorld/InstncG2.h"
#include "cdc/dtp/objectbasedata.h"
#include "UberObject.h"

#if ENABLE_IMGUI
#include "cdcResource/ResolveSection.h"
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

	instance->owner = parent;
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

void UberObjectComposite::notifyEntry(UberObjectSection *section, uint32_t state) {
	// TODO: call methodC
	printf("notifyEntry %d\n", state);
}

void UberObjectComposite::notifyExit(UberObjectSection *section, uint32_t state) {
	// TODO: call methodC
	printf("notifyExit %d\n", state);
}

void UberObjectComposite::notifyTransition(UberObjectSection *section, uint32_t transition) {
	// TODO: call methodC
	printf("notifyTransition %d\n", transition);
}

void UberObjectComposite::notifyInterruption(UberObjectSection *section, uint32_t transition) {
	// TODO: call methodC
	printf("notifyInterruption %d\n", transition);
}

UberObjectComposite *UberObjectComposite::GetComposite(Instance *instance) {
	// confirm this
	if (auto *ud = instance->userDataComponent)
		if (auto *composite = static_cast<UberObjectComposite*>(ud->userData))
			if (composite->magic == 0xFEA51B1E)
				return composite;

	return nullptr;
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

void UberObjectComposite::Update() {
	// TODO
}

UberObjectSection::UberObjectSection(Instance *instance, UberObjectComposite *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index) :
	instance(instance)
{
	// TODO
	sectionProp = info;
	UserDataComponent::CreateIfNeeded(instance)->userData = this;
	currentState = info->initialState;
	entryActions(false);
}

void UberObjectSection::entryActions(bool b) {
	auto& state = sectionProp->states[currentState];
	if (state.numEntry || state.numCondEntry)
		runActionsLists(
			state.entry, state.numEntry,
			state.condEntry, state.numCondEntry,
			b);
	UberObjectComposite::GetComposite(instance->owner)->notifyEntry(this, currentState);
}

void UberObjectSection::exitActions(bool b) {
	auto& state = sectionProp->states[currentState];
	if (state.numExit || state.numCondExit)
		runActionsLists(
			state.exit, state.numExit,
			state.condExit, state.numCondExit,
			b);
	UberObjectComposite::GetComposite(instance->owner)->notifyExit(this, currentState);
}

void UberObjectSection::runActionsLists(
	dtp::UberObjectProp::Action *actions, uint32_t numActions,
	dtp::UberObjectProp::CondAction *condActions, uint32_t numCondActions,
	bool b
) {
	// HACK
	for (uint32_t i=0; i<numActions; i++)
		doAction(actions[i]);
	for (uint32_t i=0; i<numCondActions; i++)
		doAction(condActions[i].action);
	(void) b;
}

void UberObjectSection::process() {
	Update();
	nonVirtualUpdate();
}

void UberObjectSection::nonVirtualUpdate() {
	// TODO
	float timeDelta = 0.0333f; // instance_getTimeDelta(instance);
	takeAutomaticTransitions();
	timeInState += timeDelta;
}

void UberObjectSection::takeAutomaticTransitions() {
	dtp::UberObjectProp::StateProp& state = sectionProp->states[currentState];
	for (uint32_t i=0; i < state.numTransitions; i++) {
		auto tr = state.transitions[i];
		if (shouldTakeTransition(*tr)) {
			// doTransition(*tr);
			setState(tr->nextState, false);
		}
	}
}

dtp::UberObjectProp::Transition *UberObjectSection::GetUseTransition() {
	if (currentState < 0)
		return nullptr;
	auto& state = sectionProp->states[currentState];
	for (uint32_t i=0; i < state.numTransitions; i++) {
		auto tr = state.transitions[i];
		if (tr->dword18 == 19)
			return tr;
	}
	return nullptr;
}

bool UberObjectSection::IsUsable() {
	return GetUseTransition() != nullptr;
}

void UberObjectSection::Use() {
	setState(GetUseTransition()->nextState, false);
}

UberObjectSection *UberObjectSection::GetSection(Instance *instance) {
	// confirm this
	if (auto *ud = instance->userDataComponent)
		if (auto *section = static_cast<UberObjectSection*>(ud->userData))
			if (section->magic == 0xF0012345)
				return section;

	return nullptr;
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

void UberObjectSection::setState(uint32_t nextState, bool b) {
	// HACK
	if (currentState >= 0 && currentState < sectionProp->numStates)
		exitActions(false);
	currentState = nextState;
	timeInState = 0.0f;
	entryActions(b);
}

bool UberObjectSection::shouldTakeTransition(dtp::UberObjectProp::Transition& transition) {
	// TODO
	switch (transition.dword18) {
	case 1: {
		float timeout = *(float*)&transition.dword1C - 0.001;
		return timeInState < timeout;
	}
	case 2:
		// TODO
		return false;
	case 4:
		return false; // to prevent mysterious transitions for now
		return true;
	case 11:
		// TODO
		return false;
	case 13:
		// TODO
		return false;
	case 14:
		// TODO
		return false;
	default:
		return false;
	}
}

void UberObjectSection::doAction(dtp::UberObjectProp::Action& action) { // method 18
	printf("action %d\n", action.kind);
	switch (action.kind) {
		case 61: {
			auto *mtpu = (dtp::MaterialTextureParamUpdate*) &action.dword4;
			if (mtpu->assignInstanceParam)
				/*TODO*/;
			if (mtpu->assignTexture)
				/*TODO*/;
			if (mtpu->assignMaterial) {
				auto *rmi = ((cdc::InstanceDrawable*)instance->instanceDrawable)->getModelInstance();
				auto *material = (cdc::IMaterial*)cdc::g_resolveSections[10]->GetBasePointer(mtpu->materialId);
				rmi->setMaterial(mtpu->materialPrimGroupSelector, material);
			}
			break;
		}
		case 28: {
			if (auto *soundPlex = static_cast<dtp::SoundPlex*>(DTPDataSection::getPointer(action.dword4)))
				cdc::SOUND_StartPaused(soundPlex, /*delay=*/ 0.0f);

			break;
		}
	}
}

void UberObjectSection::Update() {
	// TODO
}

#if ENABLE_IMGUI
static void buildUI(UIActions& uiact, dtp::UberObjectProp::Action& sub18, Instance *instance=nullptr) {
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
		auto *mtpu = (dtp::MaterialTextureParamUpdate*) &sub18.dword4;
		if (instance) {
			ImGui::PushID(&sub18);
			if (ImGui::SmallButton("drawable updates")) {
				auto *rmi = ((cdc::InstanceDrawable*)instance->instanceDrawable)->getModelInstance();
				auto *material = (cdc::IMaterial*)cdc::g_resolveSections[10]->GetBasePointer(mtpu->materialId);
				rmi->setMaterial(mtpu->materialPrimGroupSelector, material);
			}
			ImGui::PopID();
		} else {
			ImGui::Text("drawable updates");
		}
		ImGui::Indent();
		if (mtpu->assignInstanceParam)
			ImGui::Text("assignInstanceParam %d", mtpu->instanceParamIndex);
		if (mtpu->assignTexture)
			ImGui::Text("assignTexture %d %d %x", mtpu->texturePrimGroupSelector, mtpu->textureSlot, mtpu->textureId);
		if (mtpu->assignMaterial)
			ImGui::Text("assignMaterial %d %x", mtpu->materialPrimGroupSelector, mtpu->materialId);
		ImGui::Unindent();
		break;
	}
	case 0: {
		ImGui::SameLine();
		ImGui::Text("nothing?");
		break;
	}
	case 28: {
		ImGui::SameLine();
		ImGui::Text("play soundplex %x", sub18.dword4);
		auto *soundPlex = static_cast<dtp::SoundPlex*>(DTPDataSection::getPointer(sub18.dword4));
		if (soundPlex) {
			std::function<void(cdc::SoundHandle)> ignore = [](cdc::SoundHandle){};
			buildUI(soundPlex, &ignore);
		}
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

static void buildUI(UIActions& uiact, dtp::UberObjectProp *uberProp, dtp::UberObjectProp::Consequence& conseq, Instance *instance=nullptr) {
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
		ImGui::Text("trigger transition %d towards %d", gti, trans.nextState);
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
		auto *sub18 = (dtp::UberObjectProp::Action*)conseq.pad;
		ImGui::Text("sub18 %d", sub18->kind);
		buildUI(uiact, *sub18, instance);
		break;
	}
	case 7: {
		uint32_t signalIndex = *(uint32_t*)(&conseq.pad[0]);
		ImGui::Text("send signal %d", signalIndex);
		break;
	}
	case 8: {
		auto *c8 = (dtp::Conseq8*)(&conseq.pad[0]);
		ImGui::Text("conseq8 %d steps", c8->numSteps);
		ImGui::Indent();
		for (uint32_t i=0; i<c8->numSteps; i++) {
			ImGui::Text("step %d type %d", i, c8->steps[i].stepAny.type);
		}
		ImGui::Unindent();
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

void buildUI(UIActions& uiact, dtp::UberObjectProp *uberProp, Instance *instance) {
	ImGui::Indent();
	for (uint32_t i=0; i < uberProp->numSections; i++) {
		auto& section = uberProp->sectionList[i];
		ImGui::Text("Section %d: modelIndex=%d initialState=%d", i, section.modelIndex, section.initialState);
		ImGui::Indent();
		for (uint32_t j=0; j < section.numStates; j++) {
			auto& state = section.states[j];
			ImGui::Text("State %d: %x %x %x", j,
				state.dword0,
				state.dword4,
				state.dword8);
			ImGui::Indent();
			for (uint32_t k=0; k < state.numTransitions; k++) {
				auto& tr = *state.transitions[k];
				ImGui::Text("transition[%d] -> state %d %d %d", k,
					tr.nextState,
					tr.dword18,
					tr.dword1C);
			}
			for (uint32_t k=0; k < state.numEntry; k++) {
				ImGui::Text("entry[%d] %d", k, state.entry[k].kind);
				buildUI(uiact, state.entry[k], instance);
			}
			for (uint32_t k=0; k < state.numExit; k++) {
				ImGui::Text("exit[%d] %d", k, state.exit[k].kind);
				buildUI(uiact, state.exit[k], instance);
			}
			for (uint32_t k=0; k < state.numCondEntry; k++) {
				ImGui::Text("cond entry[%d] %d", k, state.condEntry[k].action.kind);
				buildUI(uiact, state.condEntry[k].action, instance);
			}
			for (uint32_t k=0; k < state.numCondExit; k++) {
				ImGui::Text("cond exit[%d] %d", k, state.condExit[k].action.kind);
				buildUI(uiact, state.condExit[k].action, instance);
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
			buildUI(uiact, uberProp, cmd.consequences[j], instance);
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
