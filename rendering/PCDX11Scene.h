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
		CommonScene *parentScene,
		CommonSceneSub10 *sub10,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		/* ... */
		CommonSceneSub114 *sub114,
		/* ... */
		RenderPasses *renderPasses)
	:
		CommonScene(renderDevice, parentScene, sub10, renderTarget, depthBuffer, sub114, renderPasses),
		renderDevice(static_cast<PCDX11RenderDevice*>(renderDevice))
	{
		// TODO
		uint32_t mask = sub10->mask;
		// TODO
		drawableListsAndMasks = renderPasses->createDrawableLists(/*0,*/ mask, renderDevice->getRingBuffer());
	}

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;

	void addToDrawableList(DrawableList *drawableList);
	void addToDrawableList(DrawableList *drawableList, uint32_t);
};

}
