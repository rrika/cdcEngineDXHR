#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdc/dtp/objectproperties/intro.h"
#include "game/DeferredRenderingObject.h"
#include "game/LensFlareAndCoronaID.h"
#include "cdcResource/ResolveSection.h"
#include "cdcSound/SoundPlex.h"
#include "cdcObjects/Object.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcWorld/Instance.h"

void buildUI(DeferredRenderingExtraData *extra) {
	ImGui::Text("scale mode %d",
		extra->scaleModeE1);
	for (uint32_t i=0; i<8; i++) {
		auto& param = extra->params[i];
		ImGui::Text("param %d mode %d",
			i, param.mode);
	}
}

void buildUI(LensFlareAndCoronaExtraData *extra) {
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

void buildUI(UIActions& uiact, dtp::Intro *intro) {
	auto *objectSection = cdc::g_resolveSections[11];
	if (cdc::Object *object = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(intro->objectListIndex))) {
		uint32_t objFamily = buildUI(uiact, object);

		if (objFamily == 0x50) {
			auto *extraData = (DeferredRenderingExtraData*) intro->extraData1;
			buildUI(extraData);

		} else if (objFamily == 0x5b) {
			auto *extraData = (LensFlareAndCoronaExtraData*) intro->extraData1;
			buildUI(extraData);
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
	}
	if (ImGui::CollapsingHeader("Intro", ImGuiTreeNodeFlags_DefaultOpen)) {
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
			if (ImGui::SmallButton(i < 2 ? names[i] : "Sound"))
				cdc::SOUND_StartPaused(dtpData->sounds[i].m_plex, /*delay=*/ 0.0f);
	}
	// if (ImGui::CollapsingHeader("TransformComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
	// 	// TODO
	// }
	// if (ImGui::CollapsingHeader("ObjectComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
	// 	// TODO
	// }
	if (ImGui::CollapsingHeader("MeshComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
		cdc::MeshComponent& mc = instance->GetMeshComponent();
		dtp::Model *currentModel = mc.GetModel();
		uint32_t numModels = instance->GetNumModels();
		dtp::Model **models = instance->GetModels();

		const char *names[] = {
			"Model 0",
			"Model 1"
		};

		if (ImGui::BeginListBox("##models")) {
			for (uint32_t i=0; i<numModels; i++) {
				const bool is_selected = models[i] == currentModel;
				if (ImGui::Selectable(i < 2 ? names[i] : "Model", is_selected))
					mc.SetModel(i);
			}
			ImGui::EndListBox();
		}
	}
}
