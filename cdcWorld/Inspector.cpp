#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdc/dtp/objectproperties/intro.h"
#include "cdcAnim/Inspector.h"
#include "cdcRender/CommonMaterial.h"
#include "cdcRender/MaterialData.h"
#include "cdcResource/DTPDataSection.h"
#include "cdcResource/ResolveSection.h"
#include "cdcScene/SceneEntity.h"
#include "cdcSound/SoundPlex.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcObjects/UberObject.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "game/DeferredRenderingObject.h"
#include "game/LensFlareAndCoronaID.h"
#include "UIActions.h"

void buildUI(UIActions& uiact, dtp::IntroDataUberObject *extra) {
	if (!extra) {
		ImGui::Text("no extra data");
		return;
	}
	if (extra->doorCon)
		ImGui::Text("door connection to %d/0x%x %d", extra->doorCon->introId, extra->doorCon->introId, extra->doorCon->bool0);
	ImGui::Text("%d connections", extra->numConnections);
	for (uint32_t i=0; i<extra->numConnections; i++) {
		uint32_t *c = extra->connections + (3*i);
		ImGui::Text("%d instance %d command %d",
			c[0], c[1], c[2]);
	}
	ImGui::Text("%d startup commands", extra->numCommandIndices);
	for (uint32_t i=0; i<extra->numCommandIndices; i++)
		ImGui::Text("command %d", extra->commandIndices[i]);

	ImGui::Text("%d default programs", extra->numDefaultPrograms);
	for (uint32_t i=0; i<extra->numDefaultPrograms; i++) {
		uint32_t programDtp = extra->defaultPrograms[i];
		ImGui::Text("program %d: %x", i, programDtp);
		if (auto *program = static_cast<dtp::Program*>(DTPDataSection::getPointer(programDtp))) {
			ImGui::Indent();
			for (uint32_t j=0; j<program->numSegments; j++) {
				ImGui::Text("dtpIndex %x", program->segments[j].dtpIndex);
				struct Dummy0 { uint32_t soundPlexDtp; };
				struct Dummy1 { uint32_t dword0; Dummy0 *ptr; };
				struct Dummy2 { Dummy1 *ptr; };
				struct Dummy3 { uint32_t pad0[3]; Dummy2 *ptr; };
				auto *dummy3  = static_cast<Dummy3*>(DTPDataSection::getPointer(program->segments[j].dtpIndex));
				uint32_t soundPlexDtp = dummy3->ptr->ptr->ptr->soundPlexDtp;
				ImGui::Text("soundPlexDtp %x", soundPlexDtp);
				auto *soundPlex = static_cast<dtp::SoundPlex*>(DTPDataSection::getPointer(soundPlexDtp));
				if (soundPlex) {
					std::function<void(cdc::SoundHandle)> ignore = [](cdc::SoundHandle){};
					buildUI(soundPlex, &ignore);
				}
			}
			ImGui::Unindent();
		}
	}
}

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

void buildUI(UIActions& uiact, dtp::Intro *intro, Instance *instance) {
	auto *objectSection = cdc::g_resolveSections[11];

	if (cdc::Object *object = (cdc::Object*)objectSection->GetBasePointer(objectSection->FindResource(intro->objectListIndex))) {
		uint32_t objFamily = buildUI(uiact, object, instance);

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

		if (objFamily == 2) {
			auto *extraData = (dtp::IntroDataUberObject*) intro->extraData1;
			buildUI(uiact, extraData);

		} else if (objFamily == 0x50) {
			auto *extraData = (DeferredRenderingExtraData*) intro->extraData1;
			buildUI(uiact, extraData);

		} else if (objFamily == 0x5b) {
			auto *extraData = (LensFlareAndCoronaExtraData*) intro->extraData1;
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
		ImGui::Text("SceneEntity      %p (enabled=%d)", instance->sceneEntity, instance->sceneEntity ?  instance->sceneEntity->enabled : 2);
		ImGui::Text("InstanceDrawable %p (nodraw=%d)", instance->instanceDrawable, ((cdc::InstanceDrawable*)instance->instanceDrawable)->QueryNoDraw());
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
		buildUI(uiact, instance->intro, instance);
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

	UberObjectComposite *uberObjectComposite = nullptr;
	UberObjectSection *uberObjectSection = nullptr;
	if (UserDataComponent *u = instance->userDataComponent) {
	 	uberObjectComposite = static_cast<UberObjectComposite*>(u->userData);
		if (uberObjectComposite->magic != 0xFEA51B1E)
			uberObjectComposite = nullptr;

	 	uberObjectSection = static_cast<UberObjectSection*>(u->userData);
		if (uberObjectSection->magic != 0xF0012345)
			uberObjectSection = nullptr;
	}
	if (uberObjectComposite && ImGui::CollapsingHeader("UberObjectComposite", ImGuiTreeNodeFlags_DefaultOpen)) {
		// TODO
	}
	if (uberObjectSection && ImGui::CollapsingHeader("UberObjectSection", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("time in state: %f", uberObjectSection->timeInState);
		char buffer[10];
		if (ImGui::BeginListBox("##states")) {
			for (uint32_t i=0; i<uberObjectSection->sectionProp->numStates; i++) {
				const bool is_selected = i == uberObjectSection->currentState;
				snprintf(buffer, sizeof(buffer), "State %d", i);
				if (ImGui::Selectable(buffer, is_selected)) {
					uberObjectSection->setState(i, false);
				}
			}
			ImGui::EndListBox();
		}
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
