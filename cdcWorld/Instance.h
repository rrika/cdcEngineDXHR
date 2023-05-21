#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "cdcObjects/ObjectComponent.h"

namespace cdc { struct Object; }
namespace dtp { struct Intro; }

class InstanceManager;
class NsInstance;

class Instance {
	friend class InstanceManager;

	Instance *next; // 0
	Instance *prev; // 4
	cdc::Object *object; // 8
public:
	uint16_t objectFamilyId; // 14
private:
	uint8_t flags = 0; // 18
	cdc::Vector3 position; // 20
	cdc::Euler rotation; // 30

	void *objectData; // 4C

	cdc::Object *derivedObject; // 6C

	cdc::ObjectComponent objectComponent; // 138

public:
	static Instance *IntroduceInstance(dtp::Intro *intro, int16_t streamUnitID, bool force); // line 1672
	static Instance *IntroduceInstance(dtp::Intro *intro, int16_t streamUnitID, /*GCPtr<...>*/NsInstance *scriptObject, bool force); // line 1679
	void InitCommonComponents(bool initEffects, bool unknown); // line 2822
	void DefaultInit( // line 2977
		cdc::Object *pObject,
		int modelNum,
		int uniqueID,
		dtp::Intro *pIntro,
		void /*IntroData*/ *pIntroData,
		cdc::Vector3 *pPosition,
		cdc::Euler *pRotation,
		cdc::Object *pDerivedObject,
		uint8_t flags);

	cdc::ObjectComponent& GetObjectComponent() { return objectComponent; }
};
