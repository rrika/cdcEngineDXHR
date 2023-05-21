#include "cdcObjects/Object.h"
#include "cdcObjects/ObjectManager.h"
#include "Instance.h"
#include "InstanceManager.h"

using namespace cdc;

Instance *Instance::IntroduceInstance(dtp::Intro *intro, int16_t streamUnitID, bool force) { // line 1672
	return IntroduceInstance(intro, streamUnitID, nullptr, force);
}

Instance *Instance::IntroduceInstance(dtp::Intro *intro, int16_t streamUnitID, /*GCPtr<...>*/NsInstance *scriptObject, bool force) { // line 1679
	// TODO
	ObjectTracker *objectTracker = getByObjectListIndex(intro->objectListIndex);
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
	object = pObject;
	derivedObject = pDerivedObject;

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
}
