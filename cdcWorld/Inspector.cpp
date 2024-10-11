#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdc/dtp/objectproperties/intro.h"
#include "cdcAnim/Inspector.h"
#include "cdcRender/CommonMaterial.h"
#include "cdcRender/MaterialData.h"
#include "cdcResource/ResolveSection.h"
#include "cdcScene/SceneEntity.h"
#include "cdcSound/SoundPlex.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "game/DeferredRenderingObject.h"
#include "game/LensFlareAndCoronaID.h"
#include "game/dtp/conversation.h"
#include "UIActions.h"

void buildUI(UIActions& uiact, DeferredRenderingExtraData *extra) {
	// material
	if (extra->material)
		if (ImGui::SmallButton("show override material"))
			uiact.select(extra->material);

	// scale
	ImGui::Text("scale mode %d",
		extra->scaleModeE1);
	if (extra->scaleModeE1 == 1)
		ImGui::SliderFloat("Scale", extra->scale, 0, 10000.f);
	else
		ImGui::SliderFloat3("Scale", extra->scale, 0, 10000.f);

	// packed[...]
	const char *labels_packed[] = {"packed[0]", "packed[1]", "packed[2]", "packed[3]"};
	uint8_t v[4]; uint8_t minmax[] = {0, 255};
	for (uint32_t i=0; i<4; i++) {
		memcpy(v, &extra->packedVectors[i].bytes, 4);
		ImGui::SliderScalarN(labels_packed[i], ImGuiDataType_U8, v, 4, minmax+0, minmax+1, "%u");
		memcpy(&extra->packedVectors[i].bytes, v, 4);
	}

	// plain[...]
	const char *labels_plain[] = {"plain[0]", "plain[1]", "plain[2]", "plain[3]"};
	for (uint32_t i=0; i<4; i++) {
		ImGui::SliderFloat4(labels_plain[i], extra->plainVectors[i].vec128, 0.f, 10.f);
	}

	// params
	const char *modes[] = {
		"packed[0]",
		"packed[1]",
		"packed[2]",
		"packed[3]",
		"plain[0]",
		"plain[1]",
		"plain[2]",
		"plain[3]",
		"scale", // 8
		"inverse scale", // 9
		"object to world (row 0)", // 10
		"object to world (row 1)",
		"object to world (row 2)",
		"object to world (row 3)",
		"world to camera (row 0)",
		"world to camera (row 1)",
		"world to camera (row 2)",
		"world to camera (row 3)",
		"camera to view (row 0)",
		"camera to view (row 1)",
		"camera to view (row 2)",
		"camera to view (row 3)",
		"world to object (row 0)", // 22
		"world to object (row 1)",
		"world to object (row 2)",
		"world to object (row 3)",
		"camera to world (row 0)",
		"camera to world (row 1)",
		"camera to world (row 2)",
		"camera to world (row 3)",
		"view to camera (row 0)",
		"view to camera (row 1)",
		"view to camera (row 2)",
		"view to camera (row 3)",
		"not implemented 34",
		"not implemented 35",
		"unused" // 36
	};
	const char *spaces[] = {
		"object",
		"world",
		"camera", // maybe call this view space
		"view"    // and this clip space
	};
	const char *transformMode[] = {
		"pos (mat4x4)",
		"dir (mat3x3)",
		"[not implemented]"
	};
	for (uint32_t i=0; i<8; i++) {
		auto& param = extra->params[i];
		ImGui::Text("param %d mode %2d %s",
			i, param.mode, modes[param.mode]);
		if (param.matrixP != 4 && param.matrixQ != 4)
			ImGui::Text("  transform %s %s -> %s",
				transformMode[param.multiplyMode],
				spaces[param.matrixP],
				spaces[param.matrixQ]);
	}
}

void buildUI(UIActions& uiact, LensFlareAndCoronaExtraData *extra) {
	if (extra->material)
		if (ImGui::SmallButton("show override material"))
			uiact.select(extra->material);
	ImGui::Text("matrix mode %d",
		extra->matrixMode);
	for (uint32_t i=0; i<8; i++) {
		ImGui::Text("param %d mode %d",
			i, extra->mode[i]);
	}

	ImGui::Text("texture 0 %p", extra->texture[0]);
	ImGui::Text("texture 1 %p", extra->texture[1]);
	ImGui::Text("texture 2 %p", extra->texture[2]);
	ImGui::Text("texture 3 %p", extra->texture[3]);
	ImGui::Text("material %p", extra->material);
}

struct ActorNpcExtraData {
	uint32_t pad[0x3C / 4];
	dtp::Interaction *defaultInteraction; // 3C
};

void buildUI(UIActions& uiact, dtp::ConversationGraphBase *dtpGraph); // game/Conversation/Inspector.cpp

void buildUI(UIActions& uiact, dtp::Interaction *interaction) {
	if (interaction->dword0 && interaction->dtpGraph)
		buildUI(uiact, interaction->dtpGraph);
	else if (interaction->dword0)
		ImGui::Text("no graph");
	else
		ImGui::Text("dword0 = 0");
}

void buildUI(UIActions& uiact, ActorNpcExtraData *extra) {
	ImGui::Text("default interaction:");
	if (extra->defaultInteraction) {
		ImGui::SameLine();
		buildUI(uiact, extra->defaultInteraction);
	}
}

void buildUI(UIActions& uiact, dtp::Intro *intro) {
	auto *objectSection = cdc::g_resolveSections[11];

	if (cdc::Object *object = (cdc::Object*)objectSection->GetBasePointer(objectSection->FindResource(intro->objectListIndex))) {
		uint32_t objFamily = buildUI(uiact, object);

		if (auto baseObjectListIndex = object->dtpData->baseObject_objectListIndex) {
			cdc::ObjectTracker *baseObjectTracker = cdc::getByObjectListIndex(baseObjectListIndex);
			if (cdc::Object *baseObject = baseObjectTracker->objBlob)
				buildUI(uiact, baseObject);
		}

		if (uint32_t scriptTypeID = intro->m_scriptTypeID) {
			ImGui::SameLine();
			char label[20];
			snprintf(label, 20, "Type %x", scriptTypeID);
			if (ImGui::SmallButton(label)) {
				auto *scriptSection = cdc::g_resolveSections[8];
				if (cdc::ScriptType *scriptType = (cdc::ScriptType*)scriptSection->GetBasePointer(scriptSection->FindResource(scriptTypeID)))
					uiact.select(scriptType);
			}
		}

		if (objFamily == 0x50) {
			auto *extraData = (DeferredRenderingExtraData*) intro->extraData1;
			buildUI(uiact, extraData);

		} else if (objFamily == 0x5b) {
			auto *extraData = (LensFlareAndCoronaExtraData*) intro->extraData1;
			buildUI(uiact, extraData);

		} else if (objFamily == 0x53 || objFamily == 0x5d) { // NPC, NPC with replacements
			auto *extraData = (ActorNpcExtraData*) intro->extraData1;
			buildUI(uiact, extraData);
		}
	}
}

void buildUI(UIActions& uiact, Instance *instance) {
	if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Position         %f %f %f",
			instance->position.x,
			instance->position.y,
			instance->position.z);
		ImGui::Text("SceneEntity      %p", instance->sceneEntity);
		ImGui::Text("InstanceDrawable %p", instance->instanceDrawable);
		if (instance->instanceDrawable) {
			ImGui::Text("    %s", typeid(*instance->instanceDrawable).name());
			if (auto id = dynamic_cast<cdc::InstanceDrawable*>(instance->instanceDrawable)) {
				ImGui::Text("   %d additional models", id->m_additionalModelInstances.size());
			}
		}

		if (ImGui::Button("Recreate scene entity")) {
			instance->sceneEntity->Release();
			instance->sceneEntity = nullptr;
		}
		if (ImGui::Button("Remove object")) {
			uiact.select((Instance*)nullptr);
			instance->ReallyRemoveInstance();
			return;
		}
		if (instance->ballisticComponent.m_accl.IsZero3()) {
			if (ImGui::Button("Yeet")) {
				instance->ballisticComponent.m_vel = {0, 0, 35.f};
				instance->ballisticComponent.m_accl = {0, 0, -2.5f};
			}
		} else {
			if (ImGui::Button("Stop")) {
				instance->ballisticComponent.m_vel = {0, 0, 0};
				instance->ballisticComponent.m_accl = {0, 0, 0};
			}
		}

		cdc::Object *object = instance->object;
		if (false)
			for (uint32_t i=0; i<object->numAnims; i++) {
				ImGui::Text("anim [%3d]                   %04x %04x %08x %08x", i,
					object->animations[i].animID,
					object->animations[i].word2,
					object->animations[i].dword4,
					object->animations[i].dword8);
				auto animID = object->animations[i].animID;
				auto *animSection = cdc::g_resolveSections[2];
				if (cdc::AnimFragment *anim = (cdc::AnimFragment*)animSection->GetBasePointer(animSection->FindResource(animID))) {
					buildUI(uiact, anim);
				}
			}
	}
	if (ImGui::CollapsingHeader("Intro", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("uniqueID %d/0x%x", instance->intro->uniqueID, instance->intro->uniqueID);
		buildUI(uiact, instance->intro);
	}
	if (instance->intro == nullptr && ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
		buildUI(uiact, instance->object);
	}
	if (ImGui::CollapsingHeader("SoundComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
		dtp::ObjectBaseData *dtpData = instance->object->dtpData;
		const char *names[] = {
			"Sound 0",
			"Sound 1"
		};
		for (uint32_t i=0; i<dtpData->numSounds; i++)
			if (ImGui::SmallButton(i < 2 ? names[i] : "Sound")) {
				cdc::SoundHandle sh = cdc::SOUND_StartPaused(dtpData->sounds[i].m_plex, /*delay=*/ 0.0f);
				cdc::SoundTypes::Controls3d& c3d = sh->controls3d;
				c3d.playbackType = 0x101; // 3D sound
				c3d.position[0] = instance->position.x;
				c3d.position[1] = instance->position.y;
				c3d.position[2] = instance->position.z;
			}
	}
	if (instance->animComponentV2 && ImGui::CollapsingHeader("AnimComponentV2", ImGuiTreeNodeFlags_DefaultOpen)) {
		buildUI(uiact, instance->animComponentV2);
	}
	if (ImGui::CollapsingHeader("TransformComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
		cdc::TransformComponent& tc = instance->GetTransformComponent();
		ImGui::Text("matrix %p", tc.m_matrix);
		if (tc.m_matrix) {
			for (uint32_t rowIndex=0; rowIndex<4; rowIndex++) {
				float *row = tc.m_matrix->m[rowIndex];
				ImGui::Text("%-5.3f %-5.3f %-5.3f %-5.3f",
					row[0], row[1], row[2], row[3]);
			}
		}
	}
	// if (ImGui::CollapsingHeader("ObjectComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
	// 	// TODO
	// }
	if (ImGui::CollapsingHeader("MeshComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
		cdc::MeshComponent& mc = instance->GetMeshComponent();
		dtp::Model *currentModel = mc.GetModel();
		uint32_t numModels = instance->GetNumModels();
		dtp::Model **models = instance->GetModels();

		ImGui::Text("# segments = %d", currentModel->GetNumSegments());
		ImGui::Text("# virtual segments = %d", currentModel->numVirtSegments);

		char label[10];

		if (ImGui::BeginListBox("##models")) {
			for (uint32_t i=0; i<numModels; i++) {
				snprintf(label, 10, "Model %d\n", i);
				const bool is_selected = i == mc.GetCurrentRenderModelIndex();
				if (ImGui::Selectable(label, is_selected))
					mc.SetModel(i);
			}
			ImGui::EndListBox();
		}
	}
}
