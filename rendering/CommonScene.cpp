#include "CommonScene.h"
#include "Projection.h"

namespace cdc {

CommonScene::CommonScene(
	CommonRenderDevice *renderDevice,
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
	renderTarget(renderTarget),
	parentScene(parentScene),
	nextScene(nullptr),
	prevScene(nullptr),
	depthBuffer(depthBuffer),
	numSubScenes(0)
{
	this->viewport = *viewport;
	this->globalState = *globalState;

	if (viewport->fov <= 0.0f)
		projectMatrix = cdc::BuildOrthographicLH(
			viewport->width,
			viewport->height,
			viewport->nearz,
			viewport->farz);
	else
		projectMatrix = cdc::BuildPerspectiveLH(
			viewport->fov,
			viewport->aspect,
			viewport->nearz,
			viewport->farz);

	fogScaleOffset[0] = 0.0f;
	fogScaleOffset[1] = 0.0f;
}

}