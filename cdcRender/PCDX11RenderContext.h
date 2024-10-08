#pragma once
#include <cstdint>
#include <windows.h>
#include "PCDX11InternalResource.h"

struct DXGI_MODE_DESC;
struct ID3D11Texture2D;
class IDXGISwapChain;

namespace cdc {

class PCDX11RenderDevice;
class PCDX11RenderTarget;
class PCDX11DepthBuffer;

class PCDX11RenderContext : public PCDX11InternalResource {
public:
	PCDX11RenderTarget *renderTarget14 = nullptr; // 14
	HWND hwnd; // 18
	uint32_t width = 0; // 1C, read by PCDX11RenderTexture::UpdateAbsoluteSize and PCDX11RenderDevice::getContextWidth
	uint32_t height = 0; // 20, read by PCDX11RenderTexture::UpdateAbsoluteSize and PCDX11RenderDevice::getContextHeight
	uint8_t refreshRate; // 24
	uint8_t enableTripleBuffer; // 27
	uint32_t bufferCount; // 28
	bool fullscreen; // 25
	PCDX11RenderTarget *renderTarget2C = nullptr; // 2C
	PCDX11DepthBuffer *depthBuffer = nullptr; // 30
	IDXGISwapChain *swapChain = nullptr; // 34
	ID3D11Texture2D *frameBuffer = nullptr;
	PCDX11RenderDevice *renderDevice; // 68

private:
	void fillModeDesc(DXGI_MODE_DESC&);
	bool createRenderTargets();

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
		fullscreen = false; // hack
	}

	PCDX11RenderTarget *getRenderTarget() {
		if (renderTarget14)
			return renderTarget14;
		else
			return renderTarget2C;
	}

	PCDX11DepthBuffer *getDepthBuffer() {
		return depthBuffer;
	}

	void handleResize(int32_t width, int32_t height);
	bool present();

	bool internalCreate() override;
	void internalRelease() override;
	void internalOnSettingsChanged() override;
};

}
