#include "RenderLayer.h"
#include "cdcRender/PCDX11RenderDevice.h"
#include "cdcRender/renderdevice.h"

using namespace cdc;

uint32_t g_width = 640;
uint32_t g_height = 480;

namespace cdcRenderLayer {

void Create(HWND hwnd) {
	// HACK
#ifdef _WIN32
	RECT rect;
	GetClientRect(hwnd, &rect);
	g_width = rect.right;
	g_height = rect.bottom;
#endif

	if (g_CurrentRenderer == RENDERER_DX9)
		; // createPCRenderDevice(hwnd, g_width, g_height, 0);
	if (g_CurrentRenderer == RENDERER_DX11)
		createPCDX11RenderDevice(hwnd, g_width, g_height, 0);
	// TODO: ShaderUsageDX11.bin
}

void Destroy() {
	// TODO
	if (g_renderDevice) {
		g_renderDevice->refCountDec();
		delete g_renderDevice;
		g_renderDevice = nullptr;
	}
}

}
