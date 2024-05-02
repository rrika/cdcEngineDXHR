#pragma once
#include <cstdint>
#include "cdcAnim/TransformComponent.h"
#include "cdcMath/Math.h"
#include "cdcObjects/ObjectComponent.h"
#include "cdcScript/GCObject.h"
#include "cdcWorld/InstanceDrawable.h" // for MeshComponent
#include "cdcWorld/UserDataComponent.h"

namespace cdc {
	class AnimComponentV2;
	class IDrawable;
	class SceneEntity;
	struct Object;
}

namespace dtp {
	struct Intro;
	struct Model;
}

class InstanceManager;
class NsInstance;

class Instance {
	friend class InstanceManager;

public:
	Instance *next; // 0
	Instance *prev; // 4
	cdc::Object *object; // 8
	int32_t introUniqueID; // C

	uint16_t objectFamilyId; // 14
	uint8_t flags = 0; // 18

	cdc::Vector3 position; // 20
	cdc::Euler rotation; // 30
	// scale is in transformComponent

	void *objectData; // 4C

	cdc::GCPtr<NsInstance> m_scriptObject; // 58
	dtp::Model **modelsOverride = nullptr; // 5C
	uint32_t numOverrideModels = 0; // 60
	cdc::SceneEntity *sceneEntity = nullptr; // 64
	cdc::Object *derivedObject; // 6C

	// the matrices buffer is allocated by MeshComponent::SetModel
	// the matrices are filled in by ObjectComponent::InstanceInit/G2Instance_SetTransformsToIdentity
	cdc::TransformComponent transformComponent; // 90
	dtp::Intro *intro; // 124
	void *introData; // 128
	cdc::ObjectComponent objectComponent; // 138

	cdc::MeshComponent meshComponent {this}; // 218

	cdc::IDrawable *instanceDrawable = nullptr; // 23C
	UserDataComponent *userDataComponent = nullptr; // 2E0
	cdc::AnimComponentV2 *animComponentV2 = nullptr; // 2F8

	bool enableOverridePose = false; // HACK
	std::vector<cdc::Matrix> overridePose; // HACK

public:
	static Instance *IntroduceInstance(dtp::Intro *intro, int16_t streamUnitID, bool force); // line 1672
	static Instance *IntroduceInstance(dtp::Intro *intro, int16_t streamUnitID, /*GCPtr<...>*/NsInstance *scriptObject, bool force); // line 1679
	bool ScriptAndUberInitCommon(uint32_t scriptTypeId, /*GCPtr<...>*/NsInstance *scriptObject, void *unknown); // line 2793
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
	uint32_t GetNumModels();
	dtp::Model **GetModels();

	cdc::TransformComponent& GetTransformComponent() { return transformComponent; }
	cdc::ObjectComponent& GetObjectComponent() { return objectComponent; }
	cdc::MeshComponent& GetMeshComponent() { return meshComponent; }

	void InitEditorPose(dtp::Model *model); // HACK
	void BuildEditorTransforms(cdc::Matrix *matrix); // HACK
};
