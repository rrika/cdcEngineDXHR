#pragma once

namespace cdc {
	struct DrawableList;
	struct DrawableListsAndMasks;
	struct IRenderDrawable;
}

using namespace cdc;

void buildUI(IRenderDrawable *drawable);
void buildUI(DrawableList *drawableList);
void buildUI(DrawableListsAndMasks *drawableList);
