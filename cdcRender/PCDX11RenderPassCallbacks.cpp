#include "PCDX11DeviceManager.h"
#include "PCDX11Material.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderPassCallbacks.h"
#include "PCDX11Scene.h"
#include "PCDX11StateManager.h"
#include "surfaces/PCDX11DepthBuffer.h" // for CommonDepthBuffer to PCDX11DepthBuffer cast
#include "surfaces/PCDX11RenderTarget.h" // for PCDX11RenderTarget to CommonRenderTarget cast
#include "cdcSys/Color.h"

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
	CommonRenderDevice *commonRenderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	if (drawableCount == 0)
		return false;

	// TODO
	CommonScene *scene = commonRenderDevice->scene78;
	auto *renderDevice = static_cast<PCDX11RenderDevice*>(commonRenderDevice);
	auto *stateManager = deviceManager->getStateManager();

	auto *rt = scene->getRenderTarget();
	auto *db = scene->getDepthBuffer();

	// make a new color buffer for this purpose
	rt = renderDevice->createRenderTarget(
		rt->getWidth(),
		rt->getHeight(),
		0x11 /*TODO*/, 0, 0, kTextureClassNormalMap);
	static_cast<PCDX11RenderTarget*>(rt)->getRenderTexture11()->createRenderTargetView(); // HACK
	static_cast<PCDX11DepthBuffer*>(db)->isLocked = false;

	stateManager->pushRenderTargets(
		static_cast<PCDX11RenderTarget*>(rt),
		static_cast<PCDX11DepthBuffer*>(db));

	float color[] = {0.5, 0.5, 1.0, 1.0f};
	if (priorPassesBitfield & (1 << kPassIndexNonNormalDepth)) // HACK: don't clear buffer prepared by non-normal depth pass
		renderDevice->clearRenderTargetNow(0b1, color, 1.f, 0);
	else
		renderDevice->clearRenderTargetNow(0b111, color, 1.f, 0);

	return true;
}

void PCDX11NormalPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
	CommonScene *scene = renderDevice->scene78;
	auto *stateManager = deviceManager->getStateManager();

	auto *rt = stateManager->m_renderTarget;
	auto *db = stateManager->m_depthBuffer;
	stateManager->popRenderTargets();

	static_cast<PCDX11DepthBuffer*>(db)->isLocked = true;
	scene->setSharedTextureToRenderTarget(rt, 6, 0);
	scene->setSharedTextureToDepthBuffer(db, 5);

	// TODO
	stateManager->setDepthState(D3D11_COMPARISON_LESS_EQUAL, 0);
}


bool PCDX11DeferredShadingPassCallbacks::pre(
	CommonRenderDevice *commonRenderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	CommonScene *scene = commonRenderDevice->scene78;
	auto *renderDevice = static_cast<PCDX11RenderDevice*>(commonRenderDevice);
	auto *stateManager = deviceManager->getStateManager();

	auto *rt = scene->getRenderTarget();
	auto *db = scene->getDepthBuffer();

	rt = renderDevice->dx11_createRenderTarget(
		rt->getWidth(),
		rt->getHeight(),
		/*dxgiFormat*/ 0x18, // DXGI_FORMAT_R10G10B10A2_UNORM
		/*flags=*/ 0x11 /*TODO*/,
		kTextureClass2D);
	static_cast<PCDX11RenderTarget*>(rt)->getRenderTexture11()->createRenderTargetView(); // HACK

	stateManager->pushRenderTargets(
		static_cast<PCDX11RenderTarget*>(rt),
		static_cast<PCDX11DepthBuffer*>(db));

	stateManager->setRenderTargetWriteMask(15);

	return true;
}

void PCDX11DeferredShadingPassCallbacks::post(
	CommonRenderDevice *commonRenderDevice,
	uint32_t passId)
{
	// TODO
	CommonScene *scene = commonRenderDevice->scene78;
	auto *stateManager = deviceManager->getStateManager();

	auto *rt = stateManager->m_renderTarget;
	stateManager->popRenderTargets();

	scene->setSharedTextureToRenderTarget(rt, 7, 0);
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
		static_cast<PCDX11DepthBuffer*>(scene->depthBuffer)->isLocked = false;
		uint32_t clearMode = scene->viewport.clearMode;
		// if (clearMode == 24 || clearMode == 26) {
			float color[] = {0.0f, 0.0f, 0.0f, 0.0f};
			renderDevice->clearRenderTargetNow(2, color, 1.0, 0);
		// }
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
	// StencilParams stencilSettings {
	// 	0xFF00000E,
	// 	0xFF00000E,
	// 	0x0000FFFF,
	// 	0x00000000
	// };
	// stateManager->setStencil(&stencilSettings);

	CommonScene *scene = renderDevice->scene78;
	if (scene->depthBuffer) {
		// TODO: scene->depthBuffer->byte14 = 1;
		scene->globalState.tex14[10] = scene->depthBuffer->getRenderTexture();
	}
}


bool PCDX11CompositePassCallbacks::pre(
	CommonRenderDevice *commonRenderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	auto *renderDevice = static_cast<PCDX11RenderDevice*>(commonRenderDevice);
	auto *stateManager = deviceManager->getStateManager();
	// TODO
	if (drawableCount) {
		PCDX11Material::invalidate();
		auto *scene = static_cast<PCDX11Scene*>(renderDevice->scene78);
		float color[4]; SetARGB(color, scene->m_clearColor);
		renderDevice->clearRenderTargetNow(0b1 /*TODO*/, color, 1.f,
			// even though a stencil clear value is provided, it's not enabled via the flags
			0 /*scene->getViewport().clearStencil*/);
		PCDX11BaseTexture *shadowBuffer = nullptr;
		stateManager->setTextureAndSampler(10, shadowBuffer, 0, 0.0f);
		scene->m_isUseAlphaForBloom = true; // TODO
	}
	// TODO
	return true;
}

void PCDX11CompositePassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	CommonScene *commonScene = renderDevice->scene78;
	commonScene->m_isUseAlphaForBloom = 0;
}


bool PCDX11OpaquePassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	CommonScene *commonScene = renderDevice->scene78;
	commonScene->m_isUseAlphaForBloom = 1; // TODO
	return true;
}

void PCDX11OpaquePassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	CommonScene *commonScene = renderDevice->scene78;
	commonScene->m_isUseAlphaForBloom = 0;
}


bool PCDX11TranslucentPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	CommonScene *commonScene = renderDevice->scene78;
	commonScene->m_isUseAlphaForBloom = 1; // TODO
	return true;
}

void PCDX11TranslucentPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	CommonScene *commonScene = renderDevice->scene78;
	commonScene->m_isUseAlphaForBloom = 0;
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
	CommonScene *commonScene = renderDevice->scene78;
	commonScene->m_isUseAlphaForBloom = 0;
	PCDX11Material::invalidate();
}


bool PCDX11PredatorPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	// TODO
	if (drawableCount == 0)
		return false;
	CommonScene *commonScene = renderDevice->scene78;
	commonScene->m_isUseAlphaForBloom = 1; // TODO
	return true;
}

void PCDX11PredatorPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// TODO
	CommonScene *commonScene = renderDevice->scene78;
	commonScene->m_isUseAlphaForBloom = 0;
}


bool PCDX11FullScreenFXPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	return drawableCount != 0;
}

void PCDX11FullScreenFXPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// empty
}


bool PCDX11PostFSXPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	return drawableCount != 0;
}

void PCDX11PostFSXPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	CommonScene *commonScene = renderDevice->scene78;
	PCDX11Scene *scene = static_cast<PCDX11Scene*>(commonScene);
	// TODO
	// if (scene->hasPostFsxRelated7A1) {
	// 	if (scene->postFsxRelated7A4) {
	// 		(*(void (**)(void))(*(_DWORD *)scene->postFsxRelated7A4 + 12))();
	// 		scene->postFsxRelated7A4 = 0;
	// 	}
	// 	scene->base.globalState.dword38 = 0;
	// }
}


bool PCDX11DepthDependentPassCallbacks::pre(
	CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	return drawableCount != 0;
}

void PCDX11DepthDependentPassCallbacks::post(
	CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	CommonScene *commonScene = renderDevice->scene78;
	PCDX11Scene *scene = static_cast<PCDX11Scene*>(commonScene);
	scene->updateUniforms();
}

}
