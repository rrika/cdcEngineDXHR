#include "PCDX11Scene.h"
#include "PCDX11DepthBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderTarget.h"
#include "PCDX11StateManager.h"

namespace cdc {

void PCDX11Scene::renderDrawable0() {
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

}
