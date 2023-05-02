#pragma once

namespace cdc {
	class CommonScene;
	struct DrawableList;
	struct DrawableListsAndMasks;
	struct IRenderDrawable;
	struct RenderPasses;
}


void buildUI(cdc::IRenderDrawable *drawable);
void buildUI(cdc::DrawableList *drawableList);
void buildUI(cdc::DrawableListsAndMasks *drawableList);
void buildUI(cdc::RenderPasses *renderPasses, cdc::DrawableListsAndMasks *lists);
void buildUI(cdc::CommonScene *scene);
