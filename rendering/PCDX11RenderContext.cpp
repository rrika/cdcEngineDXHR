#include "PCDX11DeviceManager.h"
#include "PCDX11RenderContext.h"
#include "surfaces/PCDX11DefaultRenderTarget.h"
#include <d3d11.h>
#include <cstdio>

namespace cdc {

void PCDX11RenderContext::fillModeDesc(DXGI_MODE_DESC& modeDesc) {
	auto *displayConfig = deviceManager->getDisplayConfig();
	modeDesc.Width = 0;
	modeDesc.Height = 0;
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	modeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // TODO
	modeDesc.RefreshRate.Numerator = displayConfig->fullscreenRefreshRate;
	modeDesc.RefreshRate.Denominator = 1;
	if (displayConfig->fullscreen) {
		modeDesc.Width = displayConfig->fullscreenWidth;
		modeDesc.Height = displayConfig->fullscreenHeight;
	} else {
		modeDesc.Width = width;
		modeDesc.Height = height;
	}
}

bool PCDX11RenderContext::createRenderTargets() {
	// TODO
	if (swapChain) {
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&frameBuffer));
	} else {
		// TODO
	}
	// TODO
	uint32_t width = 0; // TODO
	uint32_t height = 0; // TODO
	uint32_t format = 0; // TODO
	renderTarget2C = new PCDX11DefaultRenderTarget(
		width, height, 1, format, renderDevice, frameBuffer, 0);
	// TODO
	return true;
}

bool PCDX11RenderContext::present() {
	DisplayConfig *displayConfig = deviceManager->getDisplayConfig();
	uint32_t syncInterval = displayConfig->enableVsync ? 1 : 0;
	// TODO: stereo
	return swapChain && swapChain->Present(syncInterval, /*flags=*/0) >= 0;
}

bool PCDX11RenderContext::internalCreate() {
	// byte64 = 0;
	auto *device = deviceManager->getD3DDevice();
	auto *displayConfig = deviceManager->getDisplayConfig();

	DXGI_MODE_DESC modeDesc;
	fillModeDesc(modeDesc);

	if (displayConfig->fullscreen) {
		width = modeDesc.Width;
		height = modeDesc.Height;
	}

	// TODO
	// if (deviceManager->dword16C) {
	//   // stereo stuff
	// }

	auto *dxgiFactory = deviceManager->getDxgiFactory();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc = modeDesc;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = displayConfig->sampleCount;
	swapChainDesc.SampleDesc.Quality = displayConfig->sampleQuality;
	swapChainDesc.BufferUsage =
		DXGI_USAGE_UNORDERED_ACCESS |
		DXGI_USAGE_RENDER_TARGET_OUTPUT |
		DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = bufferCount = displayConfig->enableTripleBuffer ? 3 : 2;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgiFactory->CreateSwapChain((IUnknown*)device, &swapChainDesc, &swapChain);
	dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES);
	enableTripleBuffer = displayConfig->enableTripleBuffer;
	return createRenderTargets();
}

void PCDX11RenderContext::internalRelease() {
	/*TODO*/
}

void PCDX11RenderContext::internalOnSettingsChanged() {
	/*TODO*/
}

}