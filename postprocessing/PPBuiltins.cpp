#include "PPBuiltins.h"
#include "cdcRender/PCDX11DeviceManager.h"
#include "cdcRender/IRenderDrawable.h"
#include "cdcRender/drawables/PCDX11FXAADrawable.h"
#include "cdcRender/drawables/PCDX11MLAADrawable.h"
#include "cdcRender/surfaces/PCDX11RenderTarget.h"
#include "cdcRender/surfaces/PCDX11RenderTexture.h"
#include "cdcRender/PCDX11RenderDevice.h"
#include "cdcRender/renderdevice.h"

using namespace cdc;

void PPFastBlur(TextureMap *src, CommonRenderTarget *dst, uint32_t passMask, bool isHorizonalPass, bool weighted) {
	if (g_CurrentRenderer == RENDERER_DX9) {
		// empty
	} else if (g_CurrentRenderer == RENDERER_DX11) {
		auto *renderDevice = static_cast<PCDX11RenderDevice*>(g_renderDevice);
		renderDevice->fastBlur(
			/*texture*/ static_cast<PCDX11RenderTexture*>(src),
			/*renderTarget*/ static_cast<PCDX11RenderTarget*>(dst),
			passMask,
			isHorizonalPass,
			weighted);
	}
}

void PPAntiAlias(TextureMap *src, CommonRenderTarget *dst, uint32_t passMask) {
	if (g_CurrentRenderer == RENDERER_DX9) {
		// TODO
	} else if (g_CurrentRenderer == RENDERER_DX11) {
		DisplayConfig *dc = deviceManager->getDisplayConfig();
		// TODO

		auto *renderDevice = static_cast<PCDX11RenderDevice*>(g_renderDevice);
		IRenderDrawable *drawable;
		if (dc->antiAliasing == 5) {
			drawable = new (renderDevice, 0) PCDX11MLAADrawable(
				renderDevice,
				/*texture*/ static_cast<PCDX11RenderTexture*>(src),
				/*renderTarget*/ static_cast<PCDX11RenderTarget*>(dst),
				/*flags*/ 0,
				/*sortZ*/ 0.0f);
		} else {
			drawable = new (renderDevice, 0) PCDX11FXAADrawable(
				renderDevice,
				/*quality*/ dc->antiAliasing - 2,
				/*texture*/ static_cast<PCDX11RenderTexture*>(src),
				/*renderTarget*/ static_cast<PCDX11RenderTarget*>(dst),
				/*flags*/ 0,
				/*sortZ*/ 0.0f);
		}

		renderDevice->recordDrawable(drawable, passMask, 0);
	}
}
