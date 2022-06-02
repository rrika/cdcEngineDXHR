#pragma once
#include <windows.h>
#include "PCDX11InternalResource.h"

class IDXGISwapChain1;

namespace cdc {

class PCDX11RenderDevice;
class PCDX11RenderTarget;

class PCDX11RenderContext : public PCDX11InternalResource {
public:
	PCDX11RenderTarget *renderTarget14 = nullptr; // 14
	HWND hwnd; // 18
	uint32_t width; // 1C
	uint32_t height; // 20
	PCDX11RenderTarget *renderTarget2C = nullptr; // 2C
	// IDXGISwapChain *swapChain = nullptr; // 34
	IDXGISwapChain1 *swapChain = nullptr; // 34
	PCDX11RenderDevice *renderDevice; // 68
public:

	PCDX11RenderContext(
		HWND hwnd, uint32_t width, uint32_t height, uint32_t one,
		PCDX11RenderDevice *renderDevice)
	:
		hwnd(hwnd),
		width(width),
		height(height),
		renderDevice(renderDevice)
	{

	}

	PCDX11RenderTarget *getRenderTarget() {
		if (renderTarget14)
			return renderTarget14;
		else
			return renderTarget2C;
	}

	bool internalCreate() override;
	void internalRelease() override;
};

}
