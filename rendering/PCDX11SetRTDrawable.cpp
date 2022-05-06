#include "PCDX11SetRTDrawable.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11StateManager.h"

namespace cdc {

void PCDX11SetRTDrawable::renderDrawable0() {
	auto stateManager = deviceManager->getStateManager();
	stateManager->updateRenderTargets(
		renderTarget,
		depthBuffer);
	stateManager->updateViewport();
}

uint32_t PCDX11SetRTDrawable::renderDrawable4() {
	return 1;
}

}
