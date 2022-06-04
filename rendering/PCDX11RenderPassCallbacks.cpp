#include "PCDX11DeviceManager.h"
#include "PCDX11RenderPassCallbacks.h"
#include "PCDX11StateManager.h"

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
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	return true;
}

void PCDX11DepthPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
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
