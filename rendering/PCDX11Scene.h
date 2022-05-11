#pragma once
#include "PCDX11RenderDevice.h"
#include "CommonScene.h"

namespace cdc {

class PCDX11Scene : public CommonScene {
	PCDX11RenderDevice *renderDevice;
public:
	PCDX11Scene(
		CommonRenderDevice *renderDevice,
		/* ... */
		CommonScene *otherScene,
		CommonSceneSub18 *sub18,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		/* ... */
		CommonSceneSub114 *sub114,
		/* ... */
		RenderPasses *renderPasses)
	:
		CommonScene(renderDevice, otherScene, sub18, renderTarget, depthBuffer, sub114, renderPasses),
		renderDevice(static_cast<PCDX11RenderDevice*>(renderDevice))
	{
		// TODO
		uint32_t mask = sub18->mask;
		// TODO
		drawableListsAndMasks = renderPasses->createDrawableLists(/*0,*/ mask, renderDevice->getRingBuffer());
	}

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;
};

}
