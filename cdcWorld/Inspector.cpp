#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdc/dtp/objectproperties/intro.h"
#include "game/DeferredRenderingObject.h"
#include "game/LensFlareAndCoronaID.h"
#include "cdcResource/ResolveSection.h"
#include "cdcObjects/Object.h"
#include "cdcObjects/ObjectManager.h"

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
