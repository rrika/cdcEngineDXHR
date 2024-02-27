#include "cdcObjects/ObjectManager.h"
#include "Instance.h"
#include "InstanceDrawable.h"
#include "InstanceManager.h"
#include "Object.h"

using namespace cdc;

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

	return instance;
}

void Instance::InitCommonComponents(bool initEffects, bool unknown) { // line 2822
	// this runs after DefaultInit

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
