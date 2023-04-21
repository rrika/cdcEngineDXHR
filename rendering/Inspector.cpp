#include <typeinfo>
#include "../imgui/imgui.h"
#include "surfaces/PCDX11RenderTexture.h"
#include "Inspector.h"
#include "PCDX11Scene.h"
#include "RenderPasses.h"

using namespace cdc;

void buildUI(IRenderDrawable *drawable) {
	if (auto scene = dynamic_cast<PCDX11Scene*>(drawable)) {
		if (ImGui::TreeNode("scene", "scene %p", scene)) {
			ImGui::TreePop();
		}
	} else {
		ImGui::Text("drawable %p %s", drawable, typeid(*drawable).name());
	}
}

void buildUI(DrawableList *drawableList) {
	for (auto item = drawableList->first; item; item = item->next)
		buildUI(item->drawable);
}

void buildUI(DrawableListsAndMasks *drawableList) {
	auto list = drawableList->drawableLists;
	auto mask = drawableList->passMask8;
	uint32_t index = 0;
	while (mask) {
		if (mask & 1) {
			ImGui::PushID(index);
			if (ImGui::TreeNode("pass", "pass %d: list %p", index, list)) {
				buildUI(list++);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		index++;
		mask >>= 1;
	}
}

void buildUI(RenderPasses *renderPasses, DrawableListsAndMasks *lists) {
	uint32_t *reqPass;
	reqPass = renderPasses->requestedPassesScene; // TODO: shadow

	for (uint32_t passId; (passId = *reqPass) != -1; reqPass++) {
		RenderPass *pass = &renderPasses->passes[passId];
		DrawableListsAndMasks *activeLists =
			pass->passType == kLightPass ? lists->lightPasses : lists;
		if (activeLists && (activeLists->passMask8 & 1 << passId)) {
			IRenderPassCallback *callbacks = pass->callbacks;
			DrawableList *list = &activeLists->drawableLists[activeLists->compactIndices[passId]];
			ImGui::PushID(passId);
			if (ImGui::TreeNode("pass", "%s (%d)", pass->name, passId)) {
			// if (ImGui::TreeNode("pass", "pass %d (%s): list %p", passId, pass->name, list)) {
				buildUI(list);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}
}

void buildUI(CommonScene *scene) {
	for (uint32_t i=0; i<13; i++) {
		TextureMap *t = scene->globalState.tex14[i];
		ImGui::Text("global texture %d: %p", i, t);
		if (auto renderTexture = dynamic_cast<PCDX11RenderTexture*>(t)) {
			auto s = renderTexture->createShaderResourceView();
			if (s)
				ImGui::Image(s, ImVec2(256, 256));
			else
				ImGui::Text("  createShaderResourceView returned nullptr");
		}
	}
}
