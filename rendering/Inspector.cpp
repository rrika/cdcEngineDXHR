#include "../imgui/imgui.h"
#include "Inspector.h"
#include "RenderPasses.h"

void buildUI(IRenderDrawable *drawable) {
	ImGui::Text("drawable %p", drawable);
}

void buildUI(DrawableList *drawableList) {
	if (ImGui::TreeNode("list", "list %p", drawableList)) {
		for (auto item = drawableList->first; item; item = item->next)
			buildUI(item->drawable);
		ImGui::TreePop();
	}
}

void buildUI(DrawableListsAndMasks *drawableList) {
	auto list = drawableList->drawableLists;
	auto mask = drawableList->passMask8;
	uint32_t index = 0;
	while (mask) {
		if (mask & 1) {
			if (ImGui::TreeNode("pass", "pass %d", index)) {
				buildUI(list++);
				ImGui::TreePop();
			}
		}
		index++;
		mask >>= 1;
	}
}
