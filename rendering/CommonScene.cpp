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
	CommonRenderTarget *sourceColor,
	CommonDepthBuffer *sourceDepth,
	RenderGlobalState *globalState,
	/* ... */
	RenderPasses *renderPasses)
:
	renderTarget(renderTarget),
	depthBuffer(depthBuffer),
	sourceColor(sourceColor),
	sourceDepth(sourceDepth),
	parentScene(parentScene),
	nextScene(nullptr),
	prevScene(nullptr),
	numSubScenes(0)
{
	this->viewport = *viewport;
	this->globalState = *globalState;

	byte25C = 0;

	{
		// see also PCDX11RenderModelInstance::recordDrawables
		Matrix viewMatrix;
		cdc::OrthonormalInverse3x4(&viewMatrix, viewport->viewMatrix);
		float *a = viewMatrix.m[0];
		float *b = viewMatrix.m[1];
		float *c = viewMatrix.m[2];
		float cross[] = {
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0]
		};
		m_parity = cross[0] * c[0] + cross[1] * c[1] + cross[2] * c[2];
	}

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

	// TODO: find out where this is actually assigned
	fogColor[0] = 1.0f;
	fogColor[1] = 0.0f;
	fogColor[2] = 1.0f;
	fogColor[3] = 0.0f;
}

}
