#include "RenderPasses.h"
#include "IRenderDrawable.h"

namespace cdc {

void RenderPasses::addRenderPass(uint32_t arg0, uint32_t order, uint32_t sortMode, uint32_t funcSetIndex, uint32_t firstPassId) {
	// TODO
	uint32_t passId = firstPassId;
	passes[passId].order = order;
	passes[passId].sortMode = sortMode;
	passes[passId].funcSetIndex = 0;
	passes[passId].callbacks = nullptr;
}

void RenderPasses::sort(DrawableList *list, int passId) {
	RenderPass& pass = passes[passId];
	if (pass.sortMode == 0) {
		list->sortSimple();
	} else if (pass.sortMode == 1) {
		list->sortWithFunc(&comparators[pass.funcSetIndex], pass.funcSetIndex);
	}
}

void RenderPasses::draw(DrawableList *list, int passId) {
	RenderPass& pass = passes[passId];
	list->draw(&drawers[pass.funcSetIndex], pass.funcSetIndex);
}

void RenderPasses::sortAndDraw(/*uint32_t,*/ DrawableListsAndMasks *lists, CommonRenderDevice *renderDevice, uint32_t mask) {
	// TODO
	uint32_t passId = 0;
	RenderPass *pass = &passes[passId];
	IRenderPassCallback *callbacks = pass->callbacks;
	DrawableList *list = &lists->drawableLists[0];
	uint32_t drawableCount = list->itemCount;
	uint32_t priorPasses = 0;
	if (!callbacks || callbacks->pre(renderDevice, passId, drawableCount, priorPasses)) {
		priorPasses |= 1 << passId;
		if (drawableCount) {
			sort(list, passId);
			draw(list, passId);
		}
	}
	if (callbacks)
		callbacks->post(renderDevice, passId);
}

DrawableListsAndMasks *RenderPasses::createDrawableLists(/*uint32_t,*/ uint32_t mask, RingBuffer *ringBuffer) {
	// TODO
	return new DrawableListsAndMasks(this, mask, ringBuffer);
}

void DrawableList::add(IRenderDrawable *drawable) {
	itemCount++;
	// TODO: alloc from ringbuffer
	auto item = new DrawableItem;
	item->drawable = drawable;
	item->next = nullptr;
	if (last)
		last->next = item;
	last = item;
	if (!first)
		first = item;
}

void DrawableList::sortSimple() {
	// TODO
}

void DrawableList::sortWithFunc(RenderFunctionSet *funcSet, uint32_t funcSetIndex) {
	// TODO
}

void DrawableList::draw(RenderFunctionSet *funcSet, uint32_t funcSetIndex) {
	IRenderDrawable *drawable;
	DrawableItem *item = first;

	for (IRenderDrawable *prev = nullptr; item; prev = drawable) {
		drawable = item->drawable;
		bool matchPrev = prev &&
			prev->word4 == drawable->word4 &&
			prev->getVtable() == drawable->getVtable();
		funcSet->func[drawable->word4](funcSetIndex, drawable, prev);
		item = item->next;
	}
}

DrawableListsAndMasks::DrawableListsAndMasks(
	RenderPasses *renderPasses,
	/*uint32_t,*/
	uint32_t passMask,
	RingBuffer *ringBuffer)
:
	renderPasses(renderPasses),
	passMask8(passMask),
	passMaskC(passMask)
{
	// TODO: initialize other fields
	uint32_t listCount = 0;
	while (passMask) {
		listCount ++;
		passMask &= passMask-1;
	}
	// TODO: use ringbuffer
	drawableLists = new DrawableList[listCount];

	for (uint32_t i = 0; i<listCount; i++)
		drawableLists[i] = { ringBuffer, 0, 0, 0 };
}

void DrawableListsAndMasks::add(IRenderDrawable *drawable, uint32_t insertMask) {
	DrawableList *list = drawableLists;
	uint32_t advanceMask = passMask8;
	insertMask &= passMaskC;
	while (insertMask) {
		if (insertMask & 1)
			list->add(drawable);

		if (advanceMask & 1)
			list++;

		insertMask >>= 1;
		advanceMask >>= 1;
	}
}

}
