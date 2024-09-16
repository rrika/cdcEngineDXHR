#include "PPBuiltins.h"
#include "PPManager.h"
#include "PPPass.h"
#include "rendering/PCDX11RenderDevice.h"
#include "rendering/surfaces/PCDX11RenderTarget.h"

PPManager *PPManager::s_instance = nullptr;

PPManager::PPManager() {
	activeSets.reserve(16);
	unknowns2.reserve(16);
	textures.reserve(16);
	variables.reserve(16);
}

bool PPManager::prepare() {
	// TODO
	return true;
}

bool PPManager::createScene(
	cdc::CommonRenderTarget *rt,
	cdc::CommonRenderTarget *rtParticle,
	cdc::CommonRenderTarget *rtDest, // HACK: allow specifying a different target
	cdc::CommonDepthBuffer *depth,
	cdc::RenderViewport *viewport
) {
	if (!rtDest)
		rtDest = rt;

	if (!prepare())
		return false;

	if (true) { // HACK

		auto renderDevice = static_cast<cdc::PCDX11RenderDevice*>(cdc::g_renderDevice);
		cdc::CommonRenderTarget *aaDst;
		bool fastblur = false;

		if (rt == rtDest) {
			auto *tempRenderTarget = renderDevice->dx11_createRenderTarget(
				100, 100,
				fastblur ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
				0x18, cdc::kTextureClass2D);
			tempRenderTarget->getRenderTexture11()->createRenderTargetView();
			aaDst = tempRenderTarget;

		} else {
			static_cast<cdc::PCDX11RenderTarget*>(rt)->getRenderTexture11()->createRenderTargetView();
			aaDst = rtDest;

			renderDevice->finishScene();
			renderDevice->createSubScene(
				viewport,
				rtDest,
				depth,
				nullptr,
				nullptr);
			renderDevice->getScene()->debugName = "post-process";
		}

		if (fastblur)
			PPFastBlur(
				/*src=*/ rt->getRenderTexture(),
				/*dst=*/ aaDst,
				/*passMask=*/ 0x100,
				/*isHorizonalPass=*/ true,
				/*weighted=*/ false);
		else
			PPAntiAlias(
				/*src=*/ rt->getRenderTexture(),
				/*dst=*/ aaDst,
				/*passMask=*/ 0x100);

		rt = aaDst;

		if (rt != rtDest) {
			renderDevice->finishScene();
			renderDevice->createSubScene(
				viewport,
				rtDest,
				nullptr,
				rt,
				nullptr);
			renderDevice->getScene()->debugName = "copy rt to rtDest";
		}

		return true;
	}

	dtp::PPVarPassTexBlobs *vpt = activeSets[0]->varPassTex;
	dtp::PPPassBlob *passBlobs = vpt->passes;
	PPPass pass;
	for (uint32_t i=0; i < vpt->numPasses; i++) {
		pass.init(&passBlobs[i],
			textures.data(),
			textures.size(),
			variables.data(),
			variables.size());
		// TODO
	}
	// TODO
	return true;
}

void PPManager::addActiveSet(dtp::PPActiveSet *unk1, float f) {
	// TODO
}
