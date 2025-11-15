#include <cstdlib>
#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdc/dtp/objectproperties/intro.h"
#include "cdcAnim/Inspector.h"
#include "cdcLocale/localstr.h"
#include "cdcRender/CommonMaterial.h"
#include "cdcRender/MaterialData.h"
#include "cdcRender/surfaces/PCDX11Texture.h"
#include "cdcResource/DTPDataSection.h"
#include "cdcResource/ResolveSection.h"
#include "cdcScene/SceneEntity.h"
#include "cdcSound/SoundPlex.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "cdc/dtp/objecttypes/uberobject.h"
#include "game/DeferredRenderingObject.h"
#include "game/LensFlareAndCoronaID.h"
#include "game/dtp/pickup.h"
#include "UIActions.h"

void buildUI(UIActions& uiact, dtp::IntroDataUberObject *extra) {
	if (!extra) {
		ImGui::Text("no extra data");
		return;
	}
	if (extra->keycode) {
		if (auto *keycode = static_cast<dtp::Keycode*>(DTPDataSection::getPointer(extra->keycode))) {
			ImGui::Text("keycode: %s",
				keycode->localized
				? localstr_get(keycode->stridx_code)
				: keycode->code);
			uiact.origin(keycode);
		}
	}
	ImGui::Text("%d email", extra->numEMail + extra->numConditionalEMail);
	for (uint32_t i=0; i<extra->numEMail; i++) {
		uint32_t emailDtp = extra->email[i];
		if (auto *email = static_cast<dtp::EMail*>(DTPDataSection::getPointer(emailDtp))) {
			auto *from    = localstr_get(email->stridx_from);
			auto *to      = localstr_get(email->stridx_to);
			auto *subject = localstr_get(email->stridx_subject);
			auto *body    = localstr_get(email->stridx_body);
			if (ImGui::TreeNode((void*)email, "[%x] %s", emailDtp, subject)) {
				ImGui::Text("From: %s", from);
				uiact.origin((void*)email);
				ImGui::TextDisabled("To:   %s", to);
				uiact.origin((void*)email);
				ImGui::TextWrapped("%s", body);
				ImGui::NewLine();
				ImGui::TreePop();
			}
		}
		else
			ImGui::Text("missing email %d: %x", i, emailDtp);
	}
	ImGui::Text("%d default programs", extra->numDefaultPrograms);
	for (uint32_t i=0; i<extra->numDefaultPrograms; i++) {
		uint32_t programDtp = extra->defaultPrograms[i];
		ImGui::Text("program %d: %x", i, programDtp);
		if (auto *program = static_cast<dtp::TvProgram*>(DTPDataSection::getPointer(programDtp))) {
			uiact.origin((void*)program);
			ImGui::Indent();
			for (uint32_t j=0; j<program->numEntries; j++) {
				ImGui::Text("show %d: %x", j, program->entries[j].showDtpId);
				auto *show  = static_cast<dtp::TvShow*>(DTPDataSection::getPointer(program->entries[j].showDtpId));
				uiact.origin((void*)show);
				uint32_t soundPlexDtp = show->ptr->ptr->ptr->soundPlexDtp;
				ImGui::Text("soundPlexDtp %x", soundPlexDtp);
				auto *soundPlex = static_cast<dtp::SoundPlex*>(DTPDataSection::getPointer(soundPlexDtp));
				uiact.origin((void*)soundPlex);
				if (soundPlex) {
					std::function<void(cdc::SoundHandle)> ignore = [](cdc::SoundHandle){};
					buildUI(soundPlex, &ignore);
				}
			}
			ImGui::Unindent();
		}
	}
	for (uint32_t i=0; i<extra->numLoot; i++) {
		auto *loot = &extra->loot[i];
		ImGui::Text("loot %d on section %d", i, loot->sectionIndex);
		ImGui::Indent();
		for (uint32_t j=0; j<loot->numItems; j++) {
			auto pickupDtpIndex = loot->items[j].pickupDtpIndex;
			auto count = 0; // TODO
			auto *item = static_cast<dtp::Pickup*>(DTPDataSection::getPointer(pickupDtpIndex));
			if (item)
				ImGui::Text("%d x %d %s", count, pickupDtpIndex, localstr_get(item->nameStringIndex));
			else
				ImGui::Text("%d x %d (unavailable)", count, pickupDtpIndex);
		}
		ImGui::Unindent();
	}
}


void buildUI(UIActions& uiact, Billboard& bb) {
	ImGui::PushID((void*)&bb);
	if (bb.material) {
		ImGui::SameLine();
		if (ImGui::SmallButton("show material"))
			uiact.select(bb.material);
	}
	ImGui::Indent();
	ImGui::Text("uv0: %f %f  uv1: %f %f",
		bb.u0, bb.v0, bb.u1, bb.v1);
	ImGui::Text("color: %08x  mode: %d",
		bb.color, bb.mode);
	if (bb.mode == 0) {
		ImGui::SliderFloat("Oppositeness", &bb.mode0.oppositeness, 0, 1.f);
	}
	ImGui::Checkbox("Depth func less-or-equal", &bb.depthFuncLE);
	ImGui::Unindent();
	ImGui::PopID();
}

void buildUI(DistanceFade& fade) {
	float distanceRange = 5000.f;
	ImGui::Checkbox("Far fade", &fade.farFadeEnable);
	ImGui::SliderFloat2("Far fade start/range", &fade.farFadeStart, 0.f, distanceRange);
	ImGui::Checkbox("Near fade", &fade.nearFadeEnable);
	ImGui::SliderFloat2("Near fade start/range", &fade.nearFadeStart, 0.f, distanceRange);
	ImGui::SliderFloat("Near fade level", &fade.nearFadeLevel, 0.f, 1.f);

	if (fade.farFadeEnable || fade.nearFadeEnable) {
		float plotWidth = ImGui::GetWindowWidth(); // TODO: subtract border
		float plotHeight = 100.f;
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImVec2 points[6] = {
			ImVec2(0.f, 0.f),
			ImVec2(fade.nearFadeStart - fade.nearFadeWidth, 0.f),
			ImVec2(fade.nearFadeStart, 0.f),
			ImVec2(fade.farFadeStart, 0.f),
			ImVec2(fade.farFadeStart + fade.farFadeWidth, 0.f),
			ImVec2(distanceRange, 0.f)
		};
		qsort(points, 6, sizeof(ImVec2),
			[](void const *a, void const *b) -> int {
				return int(*(float const*)a-*(float const*)b);});
		for (uint32_t i=0; i<6; i++) {
			points[i].y = fade.eval(points[i].x);
			points[i].x = p.x + points[i].x * plotWidth / distanceRange;
			points[i].y = p.y + (1.f-points[i].y) * plotHeight;
		}
		draw_list->AddPolyline(points, 6, IM_COL32(255,255,0,255), false, 2.0f);
		ImGui::Dummy(ImVec2(plotWidth, plotHeight));
	}
}

void buildUI(UIActions& uiact, DeferredRenderingExtraData *extra) {
	// material
	if (extra->material)
		if (ImGui::SmallButton("show override material"))
			uiact.select(extra->material);

	// texture
	for (uint32_t i=0; i<4; i++) {
		if (auto *tex = extra->texture[i]) {
			auto *srv = static_cast<cdc::PCDX11Texture*>(tex)->createShaderResourceView();
			ImGui::Image(srv, ImVec2(64, 64));
			ImGui::SameLine();
		}
	}
	ImGui::NewLine();

	// scale
	ImGui::Text("scale mode %d",
		extra->scaleModeE1);
	if (extra->scaleModeE1 == 1 || extra->scaleModeE1 == 2)
		ImGui::SliderFloat("Scale", extra->scale, 0, 10000.f);
	else
		ImGui::SliderFloat3("Scale", extra->scale, 0, 10000.f);

	// angle
	if (extra->scaleModeE1 == 2)
		ImGui::SliderFloat("Angle", &extra->angle88, 0, 180.f);

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
		"inverse scale and cos(angle)",
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
	uint32_t i=0;
	if (extra->instanceParamFromMatrixB7) {
		for (; i<4; i++) {
			ImGui::Text("param %d mode matrix %s -> %s row %d",
				i,
				spaces[extra->sourceSpaceB5],
				spaces[extra->targetSpaceB6],
				i);
		}
	}
	for (; i<8; i++) {
		auto& param = extra->params[i];
		ImGui::Text("param %d mode %2d %s",
			i, param.mode, modes[param.mode]);
		if (param.matrixP != 4 && param.matrixQ != 4)
			ImGui::Text("  transform %s %s -> %s",
				transformMode[param.multiplyMode],
				spaces[param.matrixP],
				spaces[param.matrixQ]);
	}

	buildUI(extra->fade);

	for (uint32_t i=0; i<extra->numBillboards; i++) {
		ImGui::Text("Billboard %d", i);
		buildUI(uiact, extra->billboards[i]);
	}
}

void buildUI(UIActions& uiact, LensFlareAndCoronaExtraData *extra) {
	if (extra->material)
		if (ImGui::SmallButton("show override material"))
			uiact.select(extra->material);
	ImGui::Text("model mode %d",
		extra->modelMode);
	ImGui::BeginDisabled(extra->modelMode == 0);
	for (uint32_t i=0; i<8; i++) {
		ImGui::Text("param %d mode %d",
			i, extra->mode[i]);
	}
	ImGui::EndDisabled();

	// texture
	for (uint32_t i=0; i<4; i++) {
		if (auto *tex = extra->texture[i]) {
			auto *srv = static_cast<cdc::PCDX11Texture*>(tex)->createShaderResourceView();
			ImGui::Image(srv, ImVec2(64, 64));
			ImGui::SameLine();
		}
	}

	for (uint32_t i=0; i<extra->numBillboards; i++) {
		ImGui::Text("Billboard %d", i);
		buildUI(uiact, extra->billboards[i]);
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

		if (instance->overridePose.size() > 1)
			ImGui::Checkbox("Pose mode", &instance->enableOverridePose);

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
	if (instance->intro) {
		if (ImGui::CollapsingHeader("Intro", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("uniqueID %d/0x%x", instance->intro->uniqueID, instance->intro->uniqueID);
			buildUI(uiact, instance->intro);
		}
	} else {
		if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
			buildUI(uiact, instance->object);
		}
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
