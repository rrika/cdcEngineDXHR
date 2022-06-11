#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderPassCallbacks.h"
#include "PCDX11StateManager.h"
#include "surfaces/PCDX11DepthBuffer.h" // for CommonDepthBuffer to PCDX11DepthBuffer cast

namespace cdc {

// globals
PCDX11NormalPassCallbacks normalPassCallbacks;
PCDX11DeferredShadingPassCallbacks deferredShadingPassCallbacks;
PCDX11DepthPassCallbacks depthPassCallbacks;
PCDX11CompositePassCallbacks compositePassCallbacks;
PCDX11OpaquePassCallbacks opaquePassCallbacks;
PCDX11TranslucentPassCallbacks translucentPassCallbacks;
PCDX11AlphaBloomFSXPassCallbacks alphaBloomFSXPassCallbacks;
PCDX11PredatorPassCallbacks predatorPassCallbacks;
PCDX11FullScreenFXPassCallbacks fullScreenFXPassCallbacks;
PCDX11PostFSXPassCallbacks postFSXPassCallbacks;
PCDX11DepthDependentPassCallbacks depthDependentPassCallbacks;


bool PCDX11NormalPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11NormalPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11DeferredShadingPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11DeferredShadingPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11DepthPassCallbacks::pre(
	CommonRenderDevice *commonRenderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	if (drawableCount == 0)
		return false;

	auto *renderDevice = static_cast<PCDX11RenderDevice*>(commonRenderDevice);
	auto *stateManager = deviceManager->getStateManager();
	CommonScene *scene = renderDevice->scene78;

	if (scene->depthBuffer) {
		// TODO: scene->depthBuffer->byte14 = 0;
		uint32_t dwordC0 = scene->sub10.dwordC0;
		if (dwordC0 == 24 || dwordC0 == 26) {
			float color[] = {0.0f, 0.0f, 0.0f, 0.0f};
			renderDevice->clearRenderTargetNow(2, color, 1.0, 0);
		}
	}

	stateManager->pushRenderTargets(nullptr, static_cast<PCDX11DepthBuffer*>(scene->depthBuffer));
	stateManager->setRenderTargetWriteMask(0);
	stateManager->setDepthState(D3D11_COMPARISON_LESS_EQUAL, 1);

	return true;
}

void PCDX11DepthPassCallbacks::post(
	CommonRenderDevice *commonRenderDevice,
	uint32_t passId)
{
	auto *renderDevice = static_cast<PCDX11RenderDevice*>(commonRenderDevice);
	auto *stateManager = deviceManager->getStateManager();

	stateManager->popRenderTargets();
	// TODO:
	// StencilSettings stencilSettings {
	// 	0xFF00000E,
	// 	0xFF00000E,
	// 	0x0000FFFF,
	// 	0x00000000
	// };
	// stateManager->setStencil(&stencilSettings);

	CommonScene *scene = renderDevice->scene78;
	if (scene->depthBuffer) {
		// TODO: scene->depthBuffer->byte14 = 1;
		scene->sub114.depthRenderTexture = scene->depthBuffer->getRenderTexture();
	}
}


bool PCDX11CompositePassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	auto *stateManager = deviceManager->getStateManager();
	PCDX11BaseTexture *shadowBuffer = nullptr;
	stateManager->setTextureAndSampler(10, shadowBuffer, 0, 0.0f);
	// TODO
	return true;
}

void PCDX11CompositePassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11OpaquePassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11OpaquePassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11TranslucentPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11TranslucentPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11AlphaBloomFSXPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11AlphaBloomFSXPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11PredatorPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11PredatorPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11FullScreenFXPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11FullScreenFXPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11PostFSXPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11PostFSXPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}


bool PCDX11DepthDependentPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11DepthDependentPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
}

}
