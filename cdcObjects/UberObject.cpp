#include <cstdio>
#include "config.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcObjects/ObjectManager.h"
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
	sectionInstances = new Instance*[numSections];
	for (uint32_t i=0; i<numSections; i++) {
		sectionList[i] = 0x7FFFFFFF;
		sectionInstances[i] = nullptr;
	}

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
			sectionInstances[i] = child;
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
		Instance *targetInstance = sectionInstances[target]; // HACK (still necessary?)
		if (UserDataComponent *u = targetInstance->userDataComponent)
			if (auto uos = static_cast<UberObjectSection*>(u->userData))
				if (uos->magic == 0xF0012345)
					uos->method4(*conseq);
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
		if (shouldTakeTransition(*tr))
			return takeTransition(*tr);
	}
}

void UberObjectSection::takeTransition15(uint32_t key) {
	dtp::UberObjectProp::StateProp& state = sectionProp->states[currentState];
	for (uint32_t i=0; i < state.numTransitions; i++) {
		auto tr = state.transitions[i];
		if (tr->dword18 == 15 && tr->dword1C == key)
			return takeTransition(*tr);
	}
}

void UberObjectSection::takeTransition18(uint32_t key) {
	dtp::UberObjectProp::StateProp& state = sectionProp->states[currentState];
	for (uint32_t i=0; i < state.numTransitions; i++) {
		auto tr = state.transitions[i];
		if (tr->dword18 == 18 && tr->dword1C == key)
			return takeTransition(*tr);
	}
}

void UberObjectSection::takeTransitionIfPresent(uint32_t transitionIndex) {
	dtp::UberObjectProp::Transition& trWanted = sectionProp->transitions[transitionIndex];
	dtp::UberObjectProp::StateProp& state = sectionProp->states[currentState];
	for (uint32_t i=0; i < state.numTransitions; i++) {
		auto tr = state.transitions[i];
		if (tr == &trWanted)
			return takeTransition(*tr);
	}
}

void UberObjectSection::takeTransition(dtp::UberObjectProp::Transition& tr) {
	// TODO
	if (tr.animIndex != 0xffff)
		instance->animComponentV2->TriggerStateTransition(0, tr.animIndex, false);
	setState(tr.nextState, false);
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
	uint32_t arg = *(uint32_t*)(&conseq.pad[0]);
	switch (conseq.kind) {
	case 1:
		return setState(arg, false);
	case 2:
		return takeTransitionIfPresent(arg);
	case 3:
		return takeTransition15(arg);
	case 4:
		return takeTransition18(arg);
	case 5: {
		uint32_t polarity = conseq.pad[4];
		if (polarity)
			stateFlags |= 1u<<arg;
		else
			stateFlags &= ~(1u<<arg);
	}
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
		return timeInState >= timeout;
	}
	case 2:
		// TODO
		return false;
	case 4:
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
		case 2:
		case 3:
		case 4: {
			uint16_t anim = *(uint16_t*)(6+(uint8_t*)&action);
			bool loop = action.kind != 2;
			if (instance->animComponentV2)
				instance->animComponentV2->TriggerStateTransition(0, anim, loop);
			else
				/*TODO*/;
		}
		case 12: {
			UberObjectComposite::GetComposite(instance->owner)->commandByIndex(action.dword4);
			break;
		}
		case 28: {
			if (auto *soundPlex = static_cast<dtp::SoundPlex*>(DTPDataSection::getPointer(action.dword4)))
				cdc::SOUND_StartPaused(soundPlex, /*delay=*/ 0.0f);

			break;
		}
		case 33: {
			if (action.dword4)
				((cdc::InstanceDrawable*)instance->instanceDrawable)->DisableNoDraw();
			else
				((cdc::InstanceDrawable*)instance->instanceDrawable)->EnableNoDraw();
			break;
		}
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
	}
}

void UberObjectSection::Update() {
	// TODO
}

#if ENABLE_IMGUI
static void buildUI(UIActions& uiact, dtp::UberObjectProp::Action& action, Instance *instance=nullptr) {
	switch (action.kind) {
	case 0: {
		ImGui::SameLine();
		ImGui::Text("nothing?");
		break;
	}
	case 1: {
		ImGui::SameLine();
		ImGui::Text("set render model %d", action.dword4);
		break;
	}
	case 2:
	case 3:
	case 4: {
		uint16_t animId = *(uint16_t*)(6+(uint8_t*)&action);
		ImGui::SameLine();
		ImGui::Text("set animation %d", animId);
		break;
	}
	case 5: {
		ImGui::SameLine();
		ImGui::Text("delete this");
		break;
	}
	case 6: {
		ImGui::SameLine();
		ImGui::Text("enable no collide");
		break;
	}
	case 7:
	case 8: {
		ImGui::SameLine();
		ImGui::Text("collide related %d", action.kind);
		break;
	}
	case 12: {
		ImGui::SameLine();
		ImGui::Text("run command %d", action.dword4);
		break;
	}
	case 16: {
		ImGui::SameLine();
		ImGui::Text("take transition 15 %d in all instances", action.dword4);
		break;
	}
	case 25: {
		ImGui::SameLine();
		ImGui::Text("fx related %d", action.dword4);
		break;
	}
	case 28: {
		ImGui::SameLine();
		ImGui::Text("play soundplex %x", action.dword4);
		auto *soundPlex = static_cast<dtp::SoundPlex*>(DTPDataSection::getPointer(action.dword4));
		if (soundPlex) {
			std::function<void(cdc::SoundHandle)> ignore = [](cdc::SoundHandle){};
			buildUI(soundPlex, &ignore);
		}
		break;
	}
	case 29: {
		ImGui::SameLine();
		ImGui::Text("start sound %d", action.dword4);
		break;
	}
	case 30: {
		ImGui::SameLine();
		ImGui::Text("stop sound %d", action.dword4);
		break;
	}
	case 33: {
		ImGui::SameLine();
		ImGui::Text("set visibility? %d", action.dword4);
		break;
	}
	case 34: {
		uint32_t sectionIndex = *(uint32_t*)(4+(uint8_t*)&action);
		uint32_t bitIndex = *(uint32_t*)(8+(uint8_t*)&action);
		uint16_t polarity = *(12+(uint8_t*)&action);
		ImGui::SameLine();
		ImGui::Text("in section %d %s flag %d", sectionIndex, polarity ? "set" : "clear", bitIndex);
		break;
	}

	// dx3-specific

	case 35: {
		float *f = (float*)(4+(uint8_t*)&action);
		ImGui::SameLine();
		ImGui::Text("if no obstacle %f %f %f %f %f %f",
			f[0], f[1], f[2],
			f[3], f[4], f[5]);
		break;
	}
	case 36: {
		ImGui::SameLine();
		ImGui::Text("destroy path finding obstacles");
		break;
	}
	case 37:
	case 38:
	case 39:
	case 40: {
		ImGui::SameLine();
		ImGui::Text("scaleform related %d", action.kind);
		break;
	}
	case 54: {
		uint16_t animIndex = *(uint16_t*)(20+(uint8_t*)&action);
		ImGui::SameLine();
		ImGui::Text("set audio duration for animation in slot %d", animIndex);
		break;
	}
	case 55: {
		ImGui::SameLine();
		ImGui::Text("send signal %d", action.dword4);
		break;
	}
	case 61: {
		ImGui::SameLine();
		auto *mtpu = (dtp::MaterialTextureParamUpdate*) &action.dword4;
		if (instance) {
			ImGui::PushID(&action);
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
	case 90: {
		ImGui::SameLine();
		ImGui::Text("%s static portals", action.dword4 ? "enable" : "disable");
		break;
	}
	default: break;
	}
}

static void buildUI(UIActions& uiact, dtp::UberObjectProp::QueryProp *query) {
	ImGui::Text("Condition: ");
	ImGui::SameLine();
	if (query->invert) {
		ImGui::SameLine();
		ImGui::Text("not (");
	}
	for (uint32_t i=0; i<query->count; i++) {
		if (i > 0) {
			ImGui::SameLine();
			ImGui::Text(query->disjunction ? "OR" : "AND");
			ImGui::SameLine();
		}
		auto& sub = query->subs[i];
		switch (sub.dword4) {
		case 1: ImGui::Text("s%d.state == %d", sub.sectionIndex, sub.dwordC); break;
		case 2: ImGui::Text("s%d.alive", sub.sectionIndex); break;
		case 3: ImGui::Text("s%d.flag%d is %s", sub.sectionIndex, sub.dword8, sub.dwordC ? "cleared" : "set"); break;
		default: ImGui::Text("unknown%d", sub.dword4); break;
		}
	}
	if (query->invert) {
		ImGui::SameLine();
		ImGui::Text(")");
	}
}

static void buildUI(UIActions& uiact, dtp::Conseq8 *c8) {
	ImGui::Text("conseq8 %d steps", c8->numSteps);
	ImGui::Indent();
	for (uint32_t i=0; i<c8->numSteps; i++) {
		auto& step = c8->steps[i];
		ImGui::Text("step %d type %d", i, step.stepAny.type);
		ImGui::SameLine();
		switch (step.stepAny.type) {
		case 0: ImGui::Text("nothing"); break;
		case 1: ImGui::Text("set anim"); break;
		case 2: ImGui::Text("TODO %f %f %d",
			step.step2.float4,
			step.step2.float8,
			step.step2.byteC); break;
		case 3: ImGui::Text("assign float120=%f",
			step.step3.float4); break;
		case 4: ImGui::Text("on section %d set state %d",
			step.step4.sectionIndex,
			step.step4.stateIndex); break;
		case 5: ImGui::Text("run command %d",
			step.step5.commandIndex); break;
		case 6: ImGui::Text("after %fs spawn %s(%d)",
			30.f * step.step6.float4,
			cdc::objectName(step.step6.spawnInfo->objListIndex),
			step.step6.spawnInfo->objListIndex); break;
		case 7: ImGui::Text("physics related"); break;
		case 8: ImGui::Text("on section %d set render model %d",
			step.step8.sectionIndex,
			step.step8.renderModelIndex); break;
		case 9: ImGui::Text("physics related"); break;
		case 10: ImGui::Text("clear mesh flag %d",
			step.step10.passIndex); break;
		case 11: ImGui::Text("set mesh flag %d",
			step.step11.passIndex); break;
		case 12: ImGui::Text("fx related"); break;
		default: ImGui::Text("out-of-bounds"); break;
		}
	}
	ImGui::Unindent();
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
		auto *action = (dtp::UberObjectProp::Action*)conseq.pad;
		ImGui::Text("action %d", action->kind);
		buildUI(uiact, *action, instance);
		break;
	}
	case 7: {
		uint32_t signalIndex = *(uint32_t*)(&conseq.pad[0]);
		ImGui::Text("send signal %d", signalIndex);
		break;
	}
	case 8: {
		auto *c8 = (dtp::Conseq8*)(&conseq.pad[0]);
		buildUI(uiact, c8);
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

void buildUI(UIActions& uiact, dtp::UberObjectProp *uberProp, dtp::UberObjectProp::Command *cmd, Instance *instance) {
	for (uint32_t j=0; j < cmd->numConditions; j++) {
		buildUI(uiact, cmd->conditions[j]);
	}
	for (uint32_t j=0; j < cmd->numConsequences; j++) {
		buildUI(uiact, uberProp, cmd->consequences[j], instance);
	}
}

void buildUI(UIActions& uiact, dtp::UberObjectProp *uberProp, Instance *instance) {
	ImGui::Indent();
	for (uint32_t i=0; i < uberProp->numSections; i++) {
		auto& section = uberProp->sectionList[i];
		ImGui::Text("Section %d: modelIndex=%d initialState=%d", i, section.modelIndex, section.initialState);
		ImGui::Indent();
		{
			auto area = ImVec2(ImGui::GetContentRegionAvail().x, 270);
			ImGui::BeginChild("ChildL", area, false);
			area.x -= 16;
			area.y -= 16;
			float ix, iy, ax, ay;
			ix = ax = section.states[0].float0;
			iy = ay = section.states[0].float4;
			for (uint32_t j=1; j < section.numStates; j++) {
				ix = std::min(ix, section.states[j].float0);
				ax = std::max(ax, section.states[j].float0);
				iy = std::min(iy, section.states[j].float4);
				ay = std::max(ay, section.states[j].float4);
			}
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			const ImVec2 anchor = ImGui::GetCursorScreenPos();
			auto t = [=](ImVec2 p, bool screenSpace=false) { return ImVec2 {
				(p.x-ix) / (ax-ix) * area.x + (screenSpace ? anchor.x : 0),
				(p.y-iy) / (ay-iy) * area.y + (screenSpace ? anchor.y : 0)
			}; };
			for (uint32_t j=0; j < section.numStates; j++) {
				auto& state = section.states[j];
				ImVec2 here = t({
					state.float0,
					state.float4}, true);

				for (uint32_t k=0; k < state.numTransitions; k++) {
					auto& tr = *state.transitions[k];
					ImVec2 there = t({
						section.states[tr.nextState].float0,
						section.states[tr.nextState].float4}, true);
					draw_list->AddLine(here, there, ImGui::GetColorU32(ImGuiCol_Text));
				}
			}
			for (uint32_t j=0; j < section.numStates; j++) {
				auto& state = section.states[j];
				ImVec2 here = t({
					state.float0,
					state.float4}, false);
				ImGui::SetCursorPos(here);

				Instance *compositeI = instance->owner ? instance->owner : instance;
				Instance *sectionI = UberObjectComposite::GetComposite(compositeI)->sectionInstances[i];
				bool active = UberObjectSection::GetSection(sectionI)->currentState == j;
				if (!active)
    				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
    			char label[10];
    			snprintf(label, 10, "%d", j);
    			if (ImGui::SmallButton(label))
    				UberObjectSection::GetSection(sectionI)->setState(j, false/*TODO*/);
				if (!active)
					ImGui::PopStyleColor();
			}
			ImGui::EndChild();
		}
		for (uint32_t j=0; j < section.numStates; j++) {
			auto& state = section.states[j];
			ImGui::Text("State %d: %f %f %x", j,
				state.float0,
				state.float4,
				state.dword8);
			ImGui::Indent();
			for (uint32_t k=0; k < state.numTransitions; k++) {
				auto& tr = *state.transitions[k];
				switch (tr.dword18) {
				case 1: {
					float timeout = *(float*)&tr.dword1C;
					ImGui::Text("transition[%d] -> state %d after %f", k,
						tr.nextState,
						timeout);
					break;
				}
				case 46: {
					char *name = *(char**)&tr.dword1C;
					ImGui::Text("transition[%d] -> state %d 46 \"%s\"", k,
						tr.nextState,
						name);
					break;
				}
				default:
					ImGui::Text("transition[%d] -> state %d %d %d", k,
						tr.nextState,
						tr.dword18,
						tr.dword1C);
					break;
				}
				if (tr.animIndex != 0xffff) {
					ImGui::SameLine();
					ImVec4 red {1.0f, 0.0f, 0.0f, 1.0f};
					ImGui::TextColored(red, "with anim %d", tr.animIndex);
				}
				if (tr.dword18 == 11) {
					ImGui::Indent();
					auto queryIndex = tr.dword1C;
					if (queryIndex < uberProp->numQueries)
						buildUI(uiact, &uberProp->queryList[queryIndex]);
					else
						ImGui::Text("query index %d out of bounds", queryIndex);
					ImGui::Unindent();
				}
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
		for (uint32_t i=0; i < section.numConseq8; i++) {
			auto *conseq8 = section.conseq8List[i];
			buildUI(uiact, conseq8);
		}
		ImGui::Unindent();
	}
	for (uint32_t i=0; i < uberProp->numCommandMaps; i++) {
		auto& cmdmap = uberProp->commandMapList[i];
		ImGui::Text("CommandMap %08x -> %d", cmdmap.key, cmdmap.commandIndex);
	}
	for (uint32_t i=0; i < uberProp->numCommands; i++) {
		auto& cmd = uberProp->commandList[i];
		ImGui::Text("Command %d", i);
		ImGui::Indent();
		buildUI(uiact, uberProp, &cmd, instance);
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
