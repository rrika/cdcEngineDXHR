#pragma once

namespace cdc {
	class CommonScene;
	struct DrawableList;
	struct DrawableListsAndMasks;
	struct IRenderDrawable;
	struct RenderPasses;
}

struct UIActions;

void buildUI(UIActions& uiact, cdc::IRenderDrawable *drawable, uint32_t funcSetId);
void buildUI(UIActions& uiact, cdc::DrawableList *drawableList, uint32_t funcSetId);
void buildUI(UIActions& uiact, cdc::DrawableListsAndMasks *drawableList);
void buildUI(UIActions& uiact, cdc::RenderPasses *renderPasses, cdc::DrawableListsAndMasks *lists);
void buildUI(cdc::CommonScene *scene);

void UIBlendMode(uint32_t blendMode, uint8_t rtMask=255);
void UIPassMask(uint32_t mask);
