#include "../PCDX11DeviceManager.h"
#include "PCDX11RenderTexture.h"
#include <d3d11.h>

namespace cdc {

PCDX11RenderTexture::PCDX11RenderTexture(
	uint16_t width, uint16_t height,
	uint32_t unknown1, uint32_t isDepthBuffer,
	PCDX11RenderDevice *renderDevice, uint32_t unknown2)
:
	PCDX11BaseTexture(width, height, 0),
	shortWidth(width),
	shortHeight(height),
	isDepthBuffer(isDepthBuffer),
	renderDevice(renderDevice)
{

	// TODO
}
void PCDX11RenderTexture::ensureRenderTargetView() {
	// TODO
	auto *device = deviceManager->getD3DDevice();
	if (!view) {
		if (isDepthBuffer == false) {
			ID3D11RenderTargetView* frameBufferView;
			device->CreateRenderTargetView(resource, /*TODO*/ nullptr, &frameBufferView);
			view = frameBufferView;
		} else {
			ID3D11DepthStencilView* depthStencilView;
			device->CreateDepthStencilView(resource, /*TODO*/ nullptr, &depthStencilView);
			this->view = depthStencilView;
			this->depthStencilView = depthStencilView;
		}
	}
	// TODO
}

void PCDX11RenderTexture::ensureBuffer() {
	// TODO
	if (!view || !shaderResourceView) {
		// TODO
		ensureRenderTargetView();
	}
}

ID3D11View *PCDX11RenderTexture::getView() {
	return view;
}

void PCDX11RenderTexture::initForRenderTarget(IRenderSurface *renderSurface, uint32_t format, ID3D11Texture2D *texture) {
	// TODO
	textureFormat = format;
	resource = texture;
}


void PCDX11RenderTexture::resRegisterAtRenderDevice() {
	// TODO
}

void PCDX11RenderTexture::resFill(void* src, size_t size, size_t offset) {
	// TODO
}

char *PCDX11RenderTexture::resGetBuffer() {
	// TODO
	return nullptr;
}

void PCDX11RenderTexture::resConstruct() {
	// TODO
}


uint32_t PCDX11RenderTexture::getWidth() {
	return width;
}

uint32_t PCDX11RenderTexture::getHeight() {
	return height;
}


ID3D11Resource *PCDX11RenderTexture::getTextureResource() {
	ensureBuffer();
	return resource;
}

ID3D11ShaderResourceView *PCDX11RenderTexture::createShaderResourceView() {
	ensureBuffer();
	return shaderResourceView;
}

ID3D11UnorderedAccessView *PCDX11RenderTexture::createUnorderedAccessView() {
	ensureBuffer();
	if (!unorderedAccessView) {
		// TODO
	}
	return unorderedAccessView;
}

ID3D11RenderTargetView *PCDX11RenderTexture::createRenderTargetView() {
	ensureBuffer();
	return static_cast<ID3D11RenderTargetView*>(view);
}

ID3D11DepthStencilView *PCDX11RenderTexture::createDepthStencilView() {
	ensureBuffer();
	return depthStencilView;
}

}
