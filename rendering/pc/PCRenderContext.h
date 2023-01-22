#pragma once
#include "PCInternalResource.h"

struct IDirect3DSurface9;
struct IDirect3DSwapChain9;

namespace cdc {

class PCRenderDevice;

class PCRenderContext : public PCInternalResource {

	PCRenderDevice *renderDevice; // C
	HWND hwnd; // 10
	uint32_t width; // 14
	uint32_t height; // 18
	bool useMultiSample; // 1C
	IDirect3DSwapChain9 *swapchain = nullptr; // 28
	IDirect3DSurface9 *surface = nullptr; // 2C

public:
	PCRenderContext(HWND hwnd, uint32_t width, uint32_t height, bool useMultiSample, PCRenderDevice *renderDevice);

	void present(RECT *srcRect, RECT *dstRect, HWND hwnd);

	bool internalCreate() override;
	void internalRelease() override;
};

}
