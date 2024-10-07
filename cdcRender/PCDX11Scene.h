#pragma once
#include "PCDX11RenderDevice.h"
#include "CommonScene.h"

namespace cdc {

class PCDX11Scene : public CommonScene {
	PCDX11RenderDevice *renderDevice; // 7A8
public:

	int debugShowTempBuffer = -1;

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
		RenderPasses *renderPasses);

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;

	void updateUniforms();
	void Finalize(DrawableList *drawableList);
};

}
