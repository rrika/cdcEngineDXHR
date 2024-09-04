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

enum RenderPassType {
	kRegularPass,
	kLightPass,
	kShadowPass
};

struct RenderPass { // = cdc::RenderPassData::RenderPass
	bool active;
	uint32_t order;
	uint32_t sortMode; // 0, 1, 2
	RenderPassType passType;
	uint32_t funcSetIndex;
	IRenderPassCallback *callbacks;
	char name[64] = {0}; // debug build only
};

struct RenderPasses { // = cdc::RenderPassData
	RenderPass passes[32];
	uint32_t requestedPassesScene[33]; // = m_sceneExecutionList
	uint32_t requestedPassesShadow[33]; // = m_shadowExecutionList
	uint32_t dword408[3]; // = m_bitMasks
	uint32_t depthSortedPasses; // 414
	uint32_t activeFuncBitfield; // 418 = m_drawableTypes
	CompareFunctionSet comparators[20]; // 41C
	RenderFunctionSet drawers[20]; // 91C

	RenderPasses();
	uint32_t addRenderPass(RenderPassType passType, uint32_t order, uint32_t sortMode, uint32_t funcSetIndex, uint32_t firstPassId);
	void setRenderPassDebugName(uint32_t passId, const char *name);
	uint32_t allocFuncIndex(const char *name);
	void sort(DrawableList *list, int passId);
	void draw(DrawableList *list, int passId);
	void sortAndDraw(RenderPassType passType, DrawableListsAndMasks *lists, CommonRenderDevice *renderdevice, uint32_t mask);
	DrawableListsAndMasks *createDrawableLists(RenderPassType passType, uint32_t mask, LinearAllocator *linear);
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
	size_t size() const;
};

struct DrawableListsAndMasks { // = cdc::RenderPassesInstance
	DrawableList *drawableLists;
	RenderPasses *renderPasses;
	uint32_t passMask8;
	uint32_t passMaskC;
	RenderPassType passType; // 10
	DrawableListsAndMasks *lightPasses; // 14
	uint8_t compactIndices[32];

	DrawableListsAndMasks(RenderPasses *, RenderPassType, uint32_t, LinearAllocator *);
	void add(IRenderDrawable *drawable, uint32_t passMask);
	DrawableList *listForPass(uint32_t);
	void absorbToBack(DrawableListsAndMasks& other);
};

}
