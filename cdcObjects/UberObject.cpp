#include "cdcAnim/AnimComponentV2.h"
#include "cdcWorld/Object.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/InstanceManager.h"
#include "cdcWorld/InstncG2.h"
#include "cdc/dtp/objectbasedata.h"
#include "UberObject.h"

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
		// instance->animComponentV2 = new cdc::AnimComponentV2(instance);
		// instance->animComponentV2->Init(model);
	}

	return instance;
}

UberObjectComposite::UberObjectComposite(Instance *instance, bool deferSectionInit) {
	auto *prop = (dtp::UberObjectProp*) instance->object->data; // actually reads a cached version thereof
	UserDataComponent::CreateIfNeeded(instance)->userData = this;
	// TODO
	this->instance = instance;
	// TODO

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
		createSection(instance, info, index); // virtual call
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
		auto *conseq = command->consequences[i];
		auto target = conseq->targetIndex;
		if (target < 0 || target >= numSections)
			continue;
		if (sectionList[target] == 0x7FFFFFFF)
			continue;
		Instance *targetInstance = InstanceManager::Find(sectionList[target]);
		if (UserDataComponent *u = targetInstance->userDataComponent)
			if (auto uos = static_cast<UberObjectSection*>(u->userData))
				if (uos->magic == 0xF0012345)
					uos->method4(conseq);
	}
}

bool UberObjectComposite::methodC(dtp::UberObjectProp::Unknown& entry, int x, void *y) {
	for (uint32_t i=0; i<entry.numConditions; i++)
		if (auto *cond = entry.conditions[i])
			if (checkCondition(cond) == false)
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
