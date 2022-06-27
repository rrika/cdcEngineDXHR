#include "../imgui/imgui.h"
#include "Inspector.h"
#include "RenderPasses.h"

void buildUI(IRenderDrawable *drawable) {
	ImGui::Text("drawable %p", drawable);
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
	reqPass = renderPasses->requestedPassesA;

	for (uint32_t passId; (passId = *reqPass) != -1; reqPass++) {
		RenderPass *pass = &renderPasses->passes[passId];
		DrawableListsAndMasks *activeLists =
			pass->useOverrideLists ? lists->overrideLists14 : lists;
		if (activeLists && (activeLists->passMask8 & 1 << passId)) {
			IRenderPassCallback *callbacks = pass->callbacks;
			DrawableList *list = &activeLists->drawableLists[activeLists->compactIndices[passId]];
			ImGui::PushID(passId);
			if (ImGui::TreeNode("pass", "pass %d: list %p", passId, list)) {
				buildUI(list);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}
}