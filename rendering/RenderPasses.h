#pragma once
#include "IRenderPassCallback.h"

namespace cdc {

struct RingBuffer;
struct DrawableList;
struct DrawableListsAndMasks;
class IRenderDrawable;

using RenderFunc = bool(*)(uint32_t, IRenderDrawable*, IRenderDrawable*);

struct RenderFunctionSet { // guessed name
	RenderFunc func[16];
};

struct RenderPass { // guessed name
	bool active;
	uint32_t order;
	uint32_t sortMode; // 0, 1, 2
	// uint32_t dwordC;
	uint32_t funcSetIndex;
	IRenderPassCallback *callbacks;
};

struct RenderPasses { // guessed name
	RenderPass passes[32];
	//uint32_t requestedPassesA[32];
	//uint32_t dword380;
	//uint32_t requestedPassesB[32];
	//uint32_t dword404;
	//uint32_t dword408;
	//uint32_t dword40C;
	//uint32_t dword410;
	//uint32_t dword414;
	uint32_t activeFuncBitfield; // 418
	RenderFunctionSet comparators[20]; // 41C
	RenderFunctionSet drawers[20]; // 91C

	void addRenderPass(uint32_t arg0, uint32_t order, uint32_t sortMode, uint32_t funcSetIndex, uint32_t firstPassId);
	uint32_t allocFuncIndex(const char *name);
	void sort(DrawableList *list, int passId);
	void draw(DrawableList *list, int passId);
	void sortAndDraw(/*uint32_t,*/ DrawableListsAndMasks *lists, CommonRenderDevice *renderdevice, uint32_t mask);
	DrawableListsAndMasks *createDrawableLists(/*uint32_t,*/ uint32_t mask, RingBuffer *ringBuffer);
};

struct DrawableItem { // guessed name
	DrawableItem *next;
	IRenderDrawable *drawable;
};

struct DrawableList { // guessed name
	RingBuffer *ringBuffer;
	DrawableItem *first;
	DrawableItem *last;
	uint32_t itemCount;

	void add(IRenderDrawable *drawable);
	void sortSimple();
	void sortWithFunc(RenderFunctionSet *funcSet, uint32_t funcSetIndex);
	void draw(RenderFunctionSet *funcSet, uint32_t funcSetIndex);
	void absorbToBack(DrawableList& other);
};

struct DrawableListsAndMasks { // guessed name
	DrawableList *drawableLists;
	RenderPasses *renderPasses;
	uint32_t passMask8;
	uint32_t passMaskC;
	// uint32_t dword10;
	// DrawableListsAndMasks *overrideLists14;
	// uint32_t dword18;
	// uint32_t dword1C;
	// uint32_t dword20;
	// uint32_t dword24;
	// uint32_t dword28;
	// uint32_t dword3C;
	// uint32_t dword30;
	// uint32_t dword34;

	DrawableListsAndMasks(RenderPasses *, /*uint32_t,*/ uint32_t, RingBuffer *);
	void add(IRenderDrawable *drawable, uint32_t passMask);
	DrawableList *listForPass(uint32_t);
	void absorbToBack(DrawableListsAndMasks& other);
};

}
