#pragma once
#include "PCRenderDevice.h"
#include "rendering/CommonScene.h"

namespace cdc {

class PCScene : public CommonScene {
	PCRenderDevice *renderDevice; // 7A8
public:
	PCScene(
		PCRenderDevice *renderDevice,
		/* ... */
		CommonScene *parentScene,
		RenderViewport *viewport,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		/* ... */
		RenderGlobalState *globalState,
		/* ... */
		RenderPasses *renderPasses)
	:
		CommonScene(renderDevice, parentScene, viewport, renderTarget, depthBuffer, globalState, renderPasses),
		renderDevice(renderDevice)
	{
		// TODO
		uint32_t mask = viewport->mask;
		// TODO
		drawableListsAndMasks = renderPasses->createDrawableLists(kRegularPass, mask, renderDevice->getLinear());
	}

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;

	void SetupConstants();
};

}
