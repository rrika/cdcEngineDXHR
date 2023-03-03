#include <d3d9.h>
#include "cdcSys/Assert.h"
#include "PCDeviceManager.h"
#include "PCRenderContext.h"

namespace cdc {

PCRenderContext::PCRenderContext(HWND hwnd, uint32_t width, uint32_t height, bool useMultiSample, PCRenderDevice *renderDevice) :
	renderDevice(renderDevice),
	hwnd(hwnd),
	width(width),
	height(height),
	useMultiSample(useMultiSample)
{
	// TODO
}

void PCRenderContext::present(RECT *srcRect, RECT *dstRect, HWND hwnd) {
	// TODO
	if (swapchain)
		swapchain->Present(srcRect, dstRect, hwnd, /*pDirtyRegion=*/0, /*dwFlags=*/0);
	else
		FatalError("no swapchain");
}

bool PCRenderContext::internalCreate() {
	PCDeviceManager *deviceManager = deviceManager9;
	auto *d3d9Device = deviceManager->d3d9Device;

	d3d9Device->EvictManagedResources();

	if (!deviceManager->config1.fullscreen) {
		D3DPRESENT_PARAMETERS presentParams = deviceManager9->presentParams;

		if (!useMultiSample) {
			presentParams.MultiSampleType = D3DMULTISAMPLE_NONE; // 0
			presentParams.MultiSampleQuality = 0;
			presentParams.SwapEffect = D3DSWAPEFFECT_OVERLAY; // 3
		}

		presentParams.hDeviceWindow = hwnd;
		presentParams.EnableAutoDepthStencil = 0;
		presentParams.AutoDepthStencilFormat = D3DFMT_UNKNOWN; // 0
		presentParams.BackBufferWidth = width ? width : 1;
		presentParams.BackBufferHeight = height ? height : 1;
		presentParams.Windowed = 1;
		presentParams.BackBufferFormat = D3DFMT_UNKNOWN; // TODO: populate from adapterInfo

		// d3d9Device->GetSwapChain(0, &swapchain);

		if (d3d9Device->CreateAdditionalSwapChain(&presentParams, &swapchain) >= 0) {
			swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &surface);
			if (surface) {
				// TODO: assign to default render target

				// HACK
				IDirect3DSurface9 *pBack;
				swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBack);
				d3d9Device->SetRenderTarget(0, pBack);

				return true;
			}
		} else {
			
		}
		// TODO: release device
		return false;
	}
	FatalError("fullscreen not implemented");
	return false;
}

void PCRenderContext::internalRelease() {
	// TODO
}

}
