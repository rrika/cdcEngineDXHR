#pragma once

namespace cdc {
	class CommonScene;
	struct DrawableList;
	struct DrawableListsAndMasks;
	struct IRenderDrawable;
	struct RenderPasses;
}

using namespace cdc;

void buildUI(IRenderDrawable *drawable);
void buildUI(DrawableList *drawableList);
void buildUI(DrawableListsAndMasks *drawableList);
void buildUI(RenderPasses *renderPasses, DrawableListsAndMasks *lists);
void buildUI(CommonScene *scene);
