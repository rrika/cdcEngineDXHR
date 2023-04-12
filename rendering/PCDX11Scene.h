#pragma once
#include "PCDX11RenderDevice.h"
#include "CommonScene.h"

namespace cdc {

class PCDX11Scene : public CommonScene {
	PCDX11RenderDevice *renderDevice; // 7A8
public:
	PCDX11Scene(
		CommonRenderDevice *renderDevice,
		/* ... */
		CommonScene *parentScene,
		RenderViewport *viewport,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		/* ... */
		CommonRenderTarget *sourceColor,
		CommonDepthBuffer *sourceDepth,
		RenderGlobalState *globalState,
		/* ... */
		RenderPasses *renderPasses)
	:
		CommonScene(renderDevice, parentScene, viewport, renderTarget, depthBuffer, sourceColor, sourceDepth, globalState, renderPasses),
		renderDevice(static_cast<PCDX11RenderDevice*>(renderDevice))
	{
		// TODO
		uint32_t mask = viewport->mask;
		// TODO
		drawableListsAndMasks = renderPasses->createDrawableLists(kRegularPass, mask, renderDevice->getLinear());
	}

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;

	void updateUniforms();
	void addToDrawableList(DrawableList *drawableList);
	void addToDrawableList(DrawableList *drawableList, uint32_t);
};

}
