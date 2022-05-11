#include "PCDX11SetRTDrawable.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11StateManager.h"

namespace cdc {

void PCDX11SetRTDrawable::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	auto stateManager = deviceManager->getStateManager();
	stateManager->updateRenderTargets(
		renderTarget,
		depthBuffer);
	stateManager->updateViewport();
}

uint32_t PCDX11SetRTDrawable::compare(uint32_t funcSetIndex, IRenderDrawable *other) {
	return 1;
}

}
