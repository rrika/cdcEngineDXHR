#pragma once
#include "IRenderPassCallback.h"

namespace cdc {

class IRenderDrawable;
class LinearAllocator;
struct DrawableList;
struct DrawableListsAndMasks;

using DrawableCompareFn = bool(*)(uint32_t, IRenderDrawable*, IRenderDrawable*);
using DrawableRenderFn = void(*)(uint32_t, IRenderDrawable*, IRenderDrawable*);

struct CompareFunctionSet {
	DrawableCompareFn func[16];
};

struct RenderFunctionSet {
	DrawableRenderFn func[16];
};

struct RenderPass { // = cdc::RenderPassData::RenderPass
	bool active;
	uint32_t order;
	uint32_t sortMode; // 0, 1, 2
	uint32_t useOverrideLists;
	uint32_t funcSetIndex;
	IRenderPassCallback *callbacks;
};

struct RenderPasses { // = cdc::RenderPassData
	RenderPass passes[32];
	uint32_t requestedPassesA[32];
	//uint32_t dword380;
	uint32_t requestedPassesB[32];
	//uint32_t dword404;
	uint32_t dword408[3];
	uint32_t dword414;
	uint32_t activeFuncBitfield; // 418
	CompareFunctionSet comparators[20]; // 41C
	RenderFunctionSet drawers[20]; // 91C

	RenderPasses();
	uint32_t addRenderPass(uint32_t arg0, uint32_t order, uint32_t sortMode, uint32_t funcSetIndex, uint32_t firstPassId);
	uint32_t allocFuncIndex(const char *name);
	void sort(DrawableList *list, int passId);
	void draw(DrawableList *list, int passId);
	void sortAndDraw(/*uint32_t,*/ DrawableListsAndMasks *lists, CommonRenderDevice *renderdevice, uint32_t mask);
	DrawableListsAndMasks *createDrawableLists(/*uint32_t,*/ uint32_t mask, LinearAllocator *linear);
};

struct DrawableItem { // = cdc::RenderDrawableList::Node
	DrawableItem *next;
	IRenderDrawable *drawable;
};

struct DrawableList { // = cdc::RenderDrawableList
	LinearAllocator *linear;
	DrawableItem *first;
	DrawableItem *last;
	uint32_t itemCount;

	void add(IRenderDrawable *drawable);
	void sortSimple();
	void sortWithFunc(CompareFunctionSet *funcSet, uint32_t funcSetIndex);
	void draw(RenderFunctionSet *funcSet, uint32_t funcSetIndex);
	void absorbToBack(DrawableList& other);
};

struct DrawableListsAndMasks { // = cdc::RenderPassesInstance
	DrawableList *drawableLists;
	RenderPasses *renderPasses;
	uint32_t passMask8;
	uint32_t passMaskC;
	uint32_t passList;
	DrawableListsAndMasks *overrideLists14;
	uint8_t compactIndices[32];

	DrawableListsAndMasks(RenderPasses *, /*uint32_t,*/ uint32_t, LinearAllocator *);
	void add(IRenderDrawable *drawable, uint32_t passMask);
	DrawableList *listForPass(uint32_t);
	void absorbToBack(DrawableListsAndMasks& other);
};

}
