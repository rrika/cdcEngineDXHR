#include "PCDX11DeviceManager.h"
#include "PCDX11RenderContext.h"
#include <d3d11_1.h>
#include <cstdio>

namespace cdc {

bool PCDX11RenderContext::internalCreate() {

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	memset((void*)&swapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC1));
	swapChainDesc.Width              = 0; // use window width
	swapChainDesc.Height             = 0; // use window height
	swapChainDesc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	swapChainDesc.Stereo             = FALSE;
	swapChainDesc.SampleDesc.Count   = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount        = 1;
	swapChainDesc.Scaling            = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags              = 0;

	auto *device = deviceManager->getD3DDevice();
	auto *dxgiFactory = deviceManager->getDxgiFactory();

	dxgiFactory->CreateSwapChainForHwnd(device, hwnd, &swapChainDesc, nullptr, nullptr, &swapChain);
	return true;
}

void PCDX11RenderContext::internalRelease() {
	/*TODO*/
}

}
