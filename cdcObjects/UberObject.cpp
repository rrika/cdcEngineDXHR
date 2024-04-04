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
		createSection(instance, info, index); // virtual call
	}
	return child;
}

UberObjectSection *UberObjectComposite::createSection(Instance *instance, dtp::UberObjectProp::SectionProp *info, uint32_t index) {
	return new UberObjectSection(instance, this, info, index);
}

UberObjectSection::UberObjectSection(Instance *instance, UberObjectComposite *composite, dtp::UberObjectProp::SectionProp *info, uint32_t index) {
	// TODO
}
