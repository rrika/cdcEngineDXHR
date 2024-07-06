#include "cdcAnim/AnimComponentV2.h"
#include "cdcNative/game/NsInstance.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcResource/ResolveSection.h"
#include "cdcResource/ScriptSection.h"
#include "cdcScript/ScriptType.h"
#include "cdcSys/Assert.h"
#include "Instance.h"
#include "InstanceDrawable.h"
#include "InstanceManager.h"
#include "Object.h"
#include "SceneLayer.h"

#include "game/dtp/modularhuman.h"
#include "rendering/CommonRenderDevice.h"
#include "rendering/IMaterial.h"

using namespace cdc;

void Instance::ReallyRemoveInstance() { // line 1086
	// TODO
	SceneLayer::RemoveInstance(this);
	// TODO
	// object->RemRef();
	// if (derivedObject) derivedObject->RemRef();
	// this->~Instance();

	// HACK
	auto& il = InstanceManager::s_instances;
	for (auto it = il.begin(); it != il.end(); ++it)
		if (*it == this) {
			il.erase(it);
			break;
		}
	for (auto **i=&InstanceManager::s_chain; *i; i = &(*i)->next)
		if (*i == this) {
			*i = this->next;
			break;
		}
	delete this;
}

Instance *Instance::IntroduceInstance(dtp::Intro *intro, int16_t streamUnitID, bool force) { // line 1672
	return IntroduceInstance(intro, streamUnitID, nullptr, force);
}

Instance *Instance::IntroduceInstance(dtp::Intro *intro, int16_t streamUnitID, /*GCPtr<...>*/NsInstance *scriptObject, bool force) { // line 1679
	// called from STREAM_LevelLoadAndInit

	if (intro->objectListIndex == 0x12) // player.drm
		return nullptr; // HACK

	// TODO
	ObjectTracker *objectTracker = getByObjectListIndex(intro->objectListIndex);
	if (objectTracker == nullptr)
		return nullptr;

	Object *object = objectTracker->objBlob;

	Object *firstObject;
	Object *secondObject;

	if (auto baseObjectListIndex = object->dtpData->baseObject_objectListIndex) {
		ObjectTracker *baseObjectTracker = getByObjectListIndex(baseObjectListIndex);
		Object *baseObject = baseObjectTracker->objBlob;

		firstObject = baseObject;
		secondObject = object;

	} else {
		firstObject = object;
		secondObject = nullptr;
	}

	Instance *instance = InstanceManager::CreateInstance(intro);
	intro->field41 |= 2;
	instance->DefaultInit(
		firstObject,
		/*modelNum=*/ 0,
		intro->uniqueID,
		intro,
		intro->extraData1,
		(cdc::Vector3*) intro->position,
		(cdc::Euler*) intro->rotation,
		secondObject,
		/*flags=*/ 0);

	// TODO

	instance->InitCommonComponents(true, true);

	// TODO

	uint32_t scriptTypeId = 0;
	if ((instance->flags & 1) == 0 && intro)
		scriptTypeId = intro->m_scriptTypeID;

	instance->ScriptAndUberInitCommon(scriptTypeId, /*TODO*/scriptObject, nullptr);

	// TODO

	return instance;
}

bool Instance::ScriptAndUberInitCommon(uint32_t scriptTypeId, /*GCPtr<...>*/NsInstance *scriptObject, void *unknown) { // line 2793
	// HACK
	auto *scriptSection = (cdc::ScriptSection*)cdc::g_resolveSections[8];
	if (auto *scriptType = scriptSection->FindScript(scriptTypeId)) {
		if (auto *scriptObject = (NsInstance*)scriptType->CreateObject()) {
			m_scriptObject = scriptObject;
			scriptObject->m_instance = this;
			return true;
		}
	}
	return false;
}

void Instance::InitCommonComponents(bool initEffects, bool unknown) { // line 2822
	// this runs after DefaultInit

	// TODO

	if (objectFamilyId != /*UberObject*/ 2) {
		// UberObjects create their own components in
		//   ObjectComponent::InstanceInit (called below)
		//   UBEROBJECT_DX3_Init
		//   UberObjectComposite_DX3
		//   CreateSections
		//   CreateSectionInstance
		//   UBEROBJECT_BirthSectionInstance

		if (object->numAnims + object->numPatterns > 0) {
			dtp::ObjectBaseData *dtpData = object->dtpData;
			if (dtpData->hasAnimGraph || dtpData->numHostedAnimGraphs) {
				animComponentV2 = new AnimComponentV2(this);
				animComponentV2->Init(meshComponent.GetBaseModel());
			}
		}
	}

	// TODO

	if (unknown)
		objectComponent.SetProcessFunctions();
	objectComponent.SetInstance(this);
	objectComponent.InstanceInit(unknown);
		// this calls GameAdditionalPostInit and creates drawables for deferred lights
	// TODO
}

void Instance::DefaultInit( // line 2977
	Object *pObject,
	int modelNum,
	int uniqueID,
	dtp::Intro *pIntro,
	void /*IntroData*/ *pIntroData,
	Vector3 *pPosition,
	Euler *pRotation,
	Object *pDerivedObject,
	uint8_t flags
) {
	// this runs before InitCommonComponents

	object = pObject;
	derivedObject = pDerivedObject;

	position = *pPosition;
	rotation = *pRotation;

	objectData = object->data;

	struct ObjProp {
		uint16_t version;
		uint16_t family;
		uint16_t id;
		uint16_t type;
	};
	objectFamilyId = 0;
	auto *objProp = (ObjProp*) objectData;
	if (objProp && objProp->id == 0xb00b)
		objectFamilyId = objProp->family;

	// TODO

	intro = pIntro;
	introData = pIntroData;

	// TODO

	meshComponent.SetModel(modelNum);

	// TODO

	if (!instanceDrawable)
		instanceDrawable = new InstanceDrawable(this);

	// HACK
	uint32_t derivedObjectFamilyId = 0;
	auto *derivedObjProp = derivedObject ? (ObjProp*) derivedObject->data : nullptr;
	if (derivedObjProp && derivedObjProp->id == 0xb00b)
		derivedObjectFamilyId = derivedObjProp->family;
	if (derivedObjectFamilyId == 93) {
		auto realInstanceDrawable = static_cast<InstanceDrawable*>(instanceDrawable);
		auto *modular = *(dtp::ModularHuman**)(0x48 + (char*)derivedObjProp);
		if (!modular) modular = (dtp::ModularHuman*)(0x8 + (char*)derivedObjProp);
		auto *rmiUnique = realInstanceDrawable->getSelectedRMI();
		if (auto *m = modular->material8)
			rmiUnique->setMaterial(m->GetId(), m);
		if (auto *m = modular->materialC)
			rmiUnique->setMaterial(m->GetId(), m);
		if (auto *m = modular->material10)
			rmiUnique->setMaterial(m->GetId(), m);
		if (modular->upperBody) {
			auto *rmiUpper = g_renderDevice->createRenderModelInstance(modular->upperBody);
			if (auto *m = modular->material18)
				rmiUpper->setMaterial(m->GetId(), m);
			if (auto *m = modular->material1C)
				rmiUpper->setMaterial(m->GetId(), m);
			realInstanceDrawable->m_additionalModelInstances.push_back({true, rmiUpper});
		}
		if (modular->hands) {
			auto *rmiHands = g_renderDevice->createRenderModelInstance(modular->hands);
			if (auto *m = modular->material24)
				rmiHands->setMaterial(m->GetId(), m);
			realInstanceDrawable->m_additionalModelInstances.push_back({true, rmiHands});
		}
		if (modular->lowerBody) {
			auto *rmiLower = g_renderDevice->createRenderModelInstance(modular->lowerBody);
			if (auto *m = modular->material2C)
				rmiLower->setMaterial(m->GetId(), m);
			if (auto *m = modular->material30)
				rmiLower->setMaterial(m->GetId(), m);
			if (auto *m = modular->material34)
				rmiLower->setMaterial(m->GetId(), m);
			realInstanceDrawable->m_additionalModelInstances.push_back({true, rmiLower});
		}
	}
}

Instance::~Instance() {
	// TODO
	delete instanceDrawable;
}

uint32_t Instance::GetNumModels() {
	if (modelsOverride)
		return numOverrideModels;

	if (derivedObject)
		return derivedObject->numModels;

	return object->numModels;
}

dtp::Model **Instance::GetModels() {
	if (modelsOverride)
		return modelsOverride;

	if (derivedObject)
		return derivedObject->models;

	return object->models;
}

void Instance::InitEditorPose(dtp::Model *model) {
	uint32_t numSegments = model->oldNumSegments;
	Segment const *modelSegments = model->GetSegmentList();

	overridePose.resize(numSegments);
	for (uint32_t i=0; i<numSegments; i++) {
		Vector pivot = modelSegments[i].pivot;
		Matrix& m = overridePose[i];
		m = identity4x4;
		m.m[3][0] = pivot.x;
		m.m[3][1] = pivot.y;
		m.m[3][2] = pivot.z;
	}
}

// see also AnimBuildTransforms
void Instance::BuildEditorTransforms(Matrix *matrices) {
	dtp::Model *model = meshComponent.GetModel();
	uint32_t numSegments = model->oldNumSegments;
	Segment const *modelSegments = model->GetSegmentList();

	for (uint32_t i=0; i<numSegments; i++)
		matrices[i] = matrices[modelSegments[i].parent] * overridePose[i];
}

void INSTANCE_parabola_update_vel_acc(Instance *instance, Vector& position, float time) {
	Vector& vel = instance->ballisticComponent.m_vel;
	Vector& accl = instance->ballisticComponent.m_accl;
	position += (accl * time * time * 0.5f) + (vel * time);
	vel += accl * time;
}

void INSTANCE_parabola_update_pos_untransformed(Instance *instance, float time, bool ignoreZ) {
	Vector delta {0.f, 0.f, 0.f, 0.f};
	INSTANCE_parabola_update_vel_acc(instance, delta, time);
	instance->position.x += delta.x;
	instance->position.y += delta.y;
	if (!ignoreZ)
		instance->position.z += delta.z;
}

// void INSTANCE_parabola_update_pos_transformed(Instance *instance, float time, bool ignoreZ) { ... }

void INSTANCE_parabola_update(Instance *instance, float time) {
	auto& tc = instance->GetTransformComponent();
	if (tc.m_matrix == nullptr)
		return;

	// TODO

	INSTANCE_parabola_update_pos_untransformed(instance, 30.f * time, /*ignoreZ=*/false);
}
