#include <typeinfo>
#include "../imgui/imgui.h"
#include "surfaces/PCDX11RenderTexture.h"
#include "CommonRenderDevice.h"
#include "Inspector.h"
#include "PCDX11Scene.h"
#include "RenderPasses.h"

using namespace cdc;

void buildUI(UIActions& uiact, IRenderDrawable *drawable, uint32_t funcSetIndex) {
	if (auto scene = dynamic_cast<PCDX11Scene*>(drawable)) {
		if (ImGui::TreeNode("scene", "scene %p", scene)) {
			ImGui::TreePop();
		}
	} else {
		ImGui::Text("drawable %p %s", drawable, typeid(*drawable).name());
		ImGui::Indent();
		ImGui::PushID(drawable);
		drawable->buildUI(funcSetIndex, uiact);
		ImGui::PopID();
		ImGui::Unindent();
	}
}

void buildUI(UIActions& uiact, DrawableList *drawableList, uint32_t funcSetIndex) {
	for (auto item = drawableList->first; item; item = item->next)
		buildUI(uiact, item->drawable, funcSetIndex);
}

void buildUI(UIActions& uiact, DrawableListsAndMasks *drawableList) {
	auto list = drawableList->drawableLists;
	auto mask = drawableList->passMask8;
	uint32_t index = 0;
	while (mask) {
		if (mask & 1) {
			ImGui::PushID(index);
			if (ImGui::TreeNode("pass", "pass %d: list %p", index, list)) {
				buildUI(uiact, list++, ~0u);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		index++;
		mask >>= 1;
	}
}

void buildUI(UIActions& uiact, RenderPasses *renderPasses, DrawableListsAndMasks *lists) {
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
				buildUI(uiact, list, pass->funcSetIndex);
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

void UIBlendMode(uint32_t blendMode, uint8_t rtMask) {

	const char *blendOps[] = {
		"ADD",
		"SUBTRACT",
		"REV_SUBTRACT",
		"MIN",
		"MAX"
	};

	const char *colorBlends[] = {
		"ZERO",
		"ONE",
		"SRC_COLOR",
		"INV_SRC_COLOR",
		"SRC_ALPHA",
		"INV_SRC_ALPHA",
		"DEST_COLOR",
		"INV_DEST_COLOR",
		"DEST_ALPHA",
		"INV_DEST_ALPHA",
		"BLEND_FACTOR",
		"INV_BLEND_FACTOR"
	};

	const char *alphaBlends[] = {
		"ZERO",
		"ONE",
		"SRC_ALPHA",
		"INV_SRC_ALPHA",
		"SRC_ALPHA",
		"INV_SRC_ALPHA",
		"DEST_ALPHA",
		"INV_DEST_ALPHA",
		"DEST_ALPHA",
		"INV_DEST_ALPHA",
		"BLEND_FACTOR",
		"INV_BLEND_FACTOR"
	};

	ImGui::Text("%08x", blendMode);
	if (!ImGui::IsItemHovered())
		return;

	ImGui::BeginTooltip();
	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 38.0f);

	bool unsureBlend = (blendMode & 1) == 0 && blendMode == 255;
	bool noBlend = (blendMode & 1) == 0 && blendMode != 255 && blendMode != 15;

	if (blendMode == 0x07010010) {
		if (unsureBlend)
			ImGui::Text("opaque, unless rt write mask != RGBA");
		else
			ImGui::Text("opaque");

	} else if (blendMode == 0x06540541)
		ImGui::Text("common blend");

	else if (unsureBlend)
		ImGui::Text("no blend, unless rt write mask != RGBA");

	if (!noBlend) {
		ImGui::Text("dst  = src*%s %s dst*%s",
			colorBlends[(blendMode >> 4) & 15],
			blendOps[(blendMode >> 1) & 7],
			colorBlends[(blendMode >> 8) & 15]
		);

		if (blendMode & 0x1000) {
			ImGui::Text("dsta = src*%s %s dst*%s",
				alphaBlends[(blendMode >> 16) & 15],
				blendOps[(blendMode >> 13) & 7],
				alphaBlends[(blendMode >> 20) & 15]);

		} else {
			ImGui::TextDisabled("dsta = src*%s %s dst*%s",
				alphaBlends[(blendMode >> 4) & 15],
				blendOps[(blendMode >> 1) & 7],
				alphaBlends[(blendMode >> 8) & 15]);
		}

		if ((blendMode & 0x7000000) == 0x7000000)
			ImGui::Text("alpha threshold = 0");
	}

	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
}

void UIPassMask(uint32_t mask) {
	ImGui::Text("%08x", mask);
	if (!ImGui::IsItemHovered())
		return;

	RenderPass *passes = g_renderDevice->renderPasses.passes;
	ImGui::BeginTooltip();
	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20.0f);
	for (uint32_t i=0; i<32; i++) {
		if (passes[i].active) {
			if ((mask >> i) & 1)
				ImGui::TextUnformatted(passes[i].name);
			else
				ImGui::TextDisabled("%s", passes[i].name);
		}
	}
	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
}
