#include <d3d11.h>
#include "../PCDX11DeviceManager.h"
#include "../PCDX11RenderContext.h"
#include "../PCDX11RenderDevice.h"
#include "PCDX11RenderTexture.h"

namespace cdc {

PCDX11RenderTexture::PCDX11RenderTexture(
	uint16_t width, uint16_t height,
	uint32_t flags, uint32_t isDepthBuffer,
	PCDX11RenderDevice *renderDevice, TextureClass shape)
:
	PCDX11BaseTexture(renderDevice, 0, width, height, /*maxFilter=*/ 1, /*wrapMode=*/ 0, shape),
	renderDevice(renderDevice),
	flags(flags),
	shortWidth(width),
	shortHeight(height),
	isDepthBuffer(isDepthBuffer)
{
	if (flags & 0x20) { // use MSAA config of display
		auto *config = deviceManager->getDisplayConfig();
		sampleCount = config->sampleCount;
		sampleQuality = config->sampleQuality;
	} else {
		sampleCount = 1;
		sampleQuality = 0;
	}

	UpdateAbsoluteSize();
}

PCDX11RenderTexture::~PCDX11RenderTexture() {
	PCDX11RenderTexture::resFree();
}

void PCDX11RenderTexture::UpdateAbsoluteSize() {
	if (flags & 8) { // relative size
		uint32_t referenceWidth, referenceHeight;

		if (flags & 0x100) {
			referenceWidth = renderDevice->getSubFrameWidth();
			referenceHeight = renderDevice->getSubFrameHeight();
		} else {
			auto *context = renderDevice->getRenderContextAny();
			referenceWidth = context->width;
			referenceHeight = context->height;
		}

		width = referenceWidth * shortWidth / 100;
		height = referenceHeight * shortHeight / 100;
	}
}

void PCDX11RenderTexture::ensureResource() {
	if (!resource) {
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = getWidth();
		desc.Height = getHeight();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = (DXGI_FORMAT) textureFormat; // from PCDX11BaseTexture
		desc.SampleDesc.Count = sampleCount;
		desc.SampleDesc.Quality = sampleQuality;
		desc.Usage = (D3D11_USAGE) 0;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		if (isDepthBuffer == false)
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
		else if (isDepthBuffer == true)
			desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // confirm

		auto *device = deviceManager->getD3DDevice();
		ID3D11Texture2D *texture;
		device->CreateTexture2D(&desc, nullptr, &texture);
		resource = texture;
		// TODO
	}
}

static DXGI_FORMAT getDepthViewFormat(DXGI_FORMAT fmt) {
	switch (fmt) {
		case DXGI_FORMAT_R24G8_TYPELESS: return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case DXGI_FORMAT_R16_TYPELESS: return DXGI_FORMAT_D16_UNORM;
		default: return DXGI_FORMAT_UNKNOWN;
	}
}

static DXGI_FORMAT getDepthShaderViewFormat(DXGI_FORMAT fmt) {
	// TODO: investigate why on wine d3d11 the depth buffer can't be used as texture in imgui
	// unless this function is made to behave like the one above.
	switch (fmt) {
		case DXGI_FORMAT_R24G8_TYPELESS: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case DXGI_FORMAT_R16_TYPELESS: return DXGI_FORMAT_R16_UNORM;
		default: return DXGI_FORMAT_UNKNOWN;
	}
}

void PCDX11RenderTexture::ensureRenderTargetView() {
	// TODO
	auto *device = deviceManager->getD3DDevice();
	if (!view) {
		if (isDepthBuffer == false) {
			D3D11_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = (DXGI_FORMAT)textureFormat;
			desc.ViewDimension = sampleCount > 1
				? D3D11_RTV_DIMENSION_TEXTURE2DMS
				: D3D11_RTV_DIMENSION_TEXTURE2D;
			ID3D11RenderTargetView* frameBufferView;
			device->CreateRenderTargetView(resource, &desc, &frameBufferView);
			view = frameBufferView;
		} else {
			D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
			desc.Format = getDepthViewFormat((DXGI_FORMAT)textureFormat);
			desc.ViewDimension = sampleCount > 1
				? D3D11_DSV_DIMENSION_TEXTURE2DMS
				: D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = 0; // read-write
			device->CreateDepthStencilView(resource, &desc, (ID3D11DepthStencilView**)&this->view);
			desc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
			device->CreateDepthStencilView(resource, &desc, &this->depthStencilView);
		}
	}

	if (!shaderResourceView)
		createShaderResourceView_internal(resource, &shaderResourceView);

	// TODO
}

void PCDX11RenderTexture::ensureBuffer() {
	// TODO
	if (!borrowedResource) {
		if (!resource) {
			ensureResource();
		}
	}
	// TODO
	if (!view || !shaderResourceView) {
		// TODO
		ensureRenderTargetView();
	}

	if (!registeredForDeletionAfterFrame && (flags & 4) == 0) {
		auto& num = renderDevice->numTemporarySurfaces;
		renderDevice->temporarySurfaces[num++] = originRenderSurface; 
		registeredForDeletionAfterFrame = true;
	}
}

ID3D11View *PCDX11RenderTexture::getView() {
	return view;
}

void PCDX11RenderTexture::initForRenderTarget(IRenderSurface *renderSurface, uint32_t format, ID3D11Texture2D *texture) {
	// TODO
	textureFormat = format;
	originRenderSurface = renderSurface;
	if (texture) {
		resource = texture;
		borrowedResource = true;
	}
}


void PCDX11RenderTexture::resFree() {
	// HACK
	if (!borrowedResource) {
		if (view)
			view->Release();
		if (depthStencilView)
			depthStencilView->Release();
		if (shaderResourceView)
			shaderResourceView->Release();
		if (resource)
			resource->Release();
	}

	if (borrowedResource) {
		if (view) {
			view->Release();
			view = nullptr;
		}

		if (shaderResourceView) {
			shaderResourceView->Release();
			shaderResourceView = nullptr;
		}

	} else {
		resource = nullptr;
	}

	if (unorderedAccessView) {
		unorderedAccessView->Release();
		unorderedAccessView = nullptr;
	}

	view = nullptr;
	shaderResourceView = nullptr;
	depthStencilView = nullptr;
	registeredForDeletionAfterFrame = false;
}

void PCDX11RenderTexture::resFill(void* src, uint32_t size, uint32_t offset) {
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
		auto *device = deviceManager->getD3DDevice();
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		desc.Format = (DXGI_FORMAT)PCDX11BaseTexture::textureFormat;
		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		device->CreateUnorderedAccessView(resource, &desc, &unorderedAccessView);
	}
	return unorderedAccessView; // wait, why not return unorderedAccessView?
}

ID3D11RenderTargetView *PCDX11RenderTexture::createRenderTargetView() {
	ensureBuffer();
	return static_cast<ID3D11RenderTargetView*>(view);
}

ID3D11DepthStencilView *PCDX11RenderTexture::createDepthStencilView() {
	ensureBuffer();
	return depthStencilView;
}

void PCDX11RenderTexture::createShaderResourceView_internal(
	ID3D11Resource *resource, ID3D11ShaderResourceView **pShaderResourceView)
{
	auto *device = deviceManager->getD3DDevice();
	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.ViewDimension = sampleCount > 1
		? D3D11_SRV_DIMENSION_TEXTURE2DMS
		: D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Format = (DXGI_FORMAT)PCDX11BaseTexture::textureFormat;
	if (isDepthBuffer)
		desc.Format = getDepthShaderViewFormat(desc.Format);
	desc.Texture2D.MipLevels = -1;

	device->CreateShaderResourceView(resource, &desc, pShaderResourceView);
}

}
