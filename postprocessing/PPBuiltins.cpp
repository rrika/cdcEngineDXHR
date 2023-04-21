#include "PPBuiltins.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/drawables/PCDX11FXAADrawable.h"
#include "rendering/surfaces/PCDX11RenderTarget.h"
#include "rendering/surfaces/PCDX11RenderTexture.h"
#include "rendering/PCDX11RenderDevice.h"
#include "rendering/renderdevice.h"

using namespace cdc;

void PPFastBlur(TextureMap *src, CommonRenderTarget *dst, uint32_t passMask /*...*/) {
	// TODO
}

void PPAntiAlias(TextureMap *src, CommonRenderTarget *dst, uint32_t passMask) {
	if (g_CurrentRenderer == RENDERER_DX9) {
		// TODO
	} else if (g_CurrentRenderer == RENDERER_DX11) {
		DisplayConfig *dc = deviceManager->getDisplayConfig();
		// TODO

		auto *renderDevice = static_cast<PCDX11RenderDevice*>(g_renderDevice);
		auto *fxaaDrawable = new (renderDevice, 0) PCDX11FXAADrawable(
			renderDevice,
			/*quality*/ dc->antiAliasing - 2,
			/*texture*/ static_cast<PCDX11RenderTexture*>(src),
			/*renderTarget*/ static_cast<PCDX11RenderTarget*>(dst),
			/*flags*/ 0,
			/*sortZ*/ 0.0f);

		renderDevice->recordDrawable(fxaaDrawable, passMask, 0);
	}
}
