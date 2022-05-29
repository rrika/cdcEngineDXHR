#pragma once
#include <windows.h>

namespace cdc {

class PCDX11RenderDevice;
class PCDX11RenderTarget;

class PCDX11RenderContext {
	PCDX11RenderTarget *renderTarget14 = nullptr; // 14
	HWND hwnd; // 18
public:
	uint32_t width; // 1C
	uint32_t height; // 20
private:
	PCDX11RenderTarget *renderTarget2C = nullptr; // 2C
	PCDX11RenderDevice *renderDevice; // 68
public:

	PCDX11RenderContext(
		HWND hwnd, uint32_t width, uint32_t height, uint32_t one,
		PCDX11RenderDevice *renderDevice)
	:
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
};

}
