#include "PCDX11Scene.h"
#include "PCDX11DepthBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderTarget.h"
#include "PCDX11StateManager.h"

namespace cdc {

void PCDX11Scene::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	auto stateManager = deviceManager->getStateManager();
	// TODO
	stateManager->pushRenderTargets(
		static_cast<PCDX11RenderTarget*>(renderTarget),
		static_cast<PCDX11DepthBuffer*>(depthBuffer));
	// TODO
	if (drawableListsAndMasks) {
		drawableListsAndMasks->renderPasses->sortAndDraw(
			drawableListsAndMasks,
			renderDevice,
			drawableListsAndMasks->passMask8);
	}
	// TODO
	stateManager->popRenderTargets();
}

void PCDX11Scene::addToDrawableList(DrawableList *drawableList) {
	// TODO
	addToDrawableList(drawableList, 0);
	// TODO
}

void PCDX11Scene::addToDrawableList(DrawableList *drawableList, uint32_t) {
	// TODO
	if (parentScene) {
		auto *parentDrawableList = parentScene->drawableListsAndMasks->listForPass(10);
		for (uint32_t i=0; i<numSubScenes; i++)
			parentDrawableList->add(subScenes[i]);
		// TODO
	} else {
		drawableList->add(this);
		for (uint32_t i=0; i<numSubScenes; i++)
			drawableList->add(subScenes[i]);
	}
}

}
