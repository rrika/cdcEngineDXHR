#include "../PCDX11DeviceManager.h"
#include "PCDX11BitmapTexture.h"
#include <d3d11.h>

namespace cdc {

PCDX11BitmapTexture::~PCDX11BitmapTexture() {
	if (d3dResourceView12C)
		d3dResourceView12C->Release();
	if (d3dTexture128)
		d3dTexture128->Release();
	// if (imageData)
}

void PCDX11BitmapTexture::CreateD3DTexture(
	DXGI_FORMAT format,
	uint16_t width,
	uint16_t height,
	uint16_t depth,
	uint16_t arraySize,
	uint8_t mipLevels,
	uint8_t **ppData,
	uint16_t wrapmode,
	const char *pName)
{
	if (d3dTexture128)
		return;

	PCDX11BaseTexture::wrapMode = wrapmode;
	PCDX11BaseTexture::textureFormat = format;
	PCDX11BaseTexture::width = width;
	PCDX11BaseTexture::height = height;
	PCDX11BaseTexture::depth = depth;
	PCDX11BaseTexture::mipLevels = mipLevels;

	PCDX11BitmapTexture::arraySize = arraySize;
	PCDX11BitmapTexture::byte124 = true;

	if (ppData) {
		// HACK
		asyncArgBypass = (char**)ppData;
		request(0);
	}
}

ID3D11Resource *PCDX11BitmapTexture::getTextureResource() {
	if (m_keepWaiting)
		awaitResource();
	return d3dTexture128;
}

ID3D11ShaderResourceView *PCDX11BitmapTexture::createShaderResourceView() {
	if (baseTexMethodC())
		baseTexMethod10();

	if (d3dResourceView12C != nullptr || d3dTexture128 == nullptr)
		return d3dResourceView12C;

	D3D11_SHADER_RESOURCE_VIEW_DESC desc {};

	if (shape == kTextureClass3D) {
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = -1;
	} else if (shape == kTextureClassCube) {
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MostDetailedMip = 0;
		desc.TextureCube.MipLevels = -1;
	} else { // kTextureClassUnknown, kTextureClass2D, kTextureClassNormalMap
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture3D.MostDetailedMip = 0;
		desc.Texture3D.MipLevels = -1;
	}

	deviceManager->getD3DDevice()->CreateShaderResourceView(d3dTexture128, &desc, &d3dResourceView12C);

	return d3dResourceView12C;
}

ID3D11UnorderedAccessView *PCDX11BitmapTexture::createUnorderedAccessView() {
	if (baseTexMethodC())
		baseTexMethod10();

	if (d3dAccessView130 != nullptr || d3dTexture128 == nullptr)
		return d3dAccessView130;

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc {};
	desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	desc.Format = (DXGI_FORMAT)textureFormat;

	deviceManager->getD3DDevice()->CreateUnorderedAccessView(d3dTexture128, &desc, &d3dAccessView130);

	return d3dAccessView130;
}

void PCDX11BitmapTexture::asyncCreate() {

	if (asyncArgBypass == nullptr)
		return; // called a second time by accident?

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc = D3D11_TEXTURE2D_DESC {};
	textureDesc.Width              = PCDX11BaseTexture::width;
	textureDesc.Height             = PCDX11BaseTexture::height;
	textureDesc.MipLevels          = PCDX11BaseTexture::mipLevels;
	textureDesc.ArraySize          = 1;
	textureDesc.Format             = (DXGI_FORMAT)PCDX11BaseTexture::textureFormat;
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.Usage              = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA *textureData = new D3D11_SUBRESOURCE_DATA[
		PCDX11BitmapTexture::arraySize *
		PCDX11BaseTexture::mipLevels];

	D3D11_SUBRESOURCE_DATA *k = textureData;
	uint32_t l = 0;
	for (auto i=0; i < PCDX11BitmapTexture::arraySize; i++)
		for (auto j=0; j < PCDX11BaseTexture::mipLevels; j++) {
			uint32_t w = PCDX11BaseTexture::width >> j;
			uint32_t h = PCDX11BaseTexture::height >> j;
			uint32_t size = PCDX11BaseTexture::StaticGetLevelSize(textureDesc.Format, w, h);
			*k++ = {
				.pSysMem = asyncArgBypass[l++],
				.SysMemPitch = PCDX11BaseTexture::StaticGetMemPitch(textureDesc.Format, w),
				.SysMemSlicePitch = size
			};
		}

	auto device = deviceManager->getD3DDevice();
	HRESULT hr = device->CreateTexture2D(&textureDesc, textureData, &d3dTexture128);
	// printf(" texture %p\n", d3dTexture128);
	// printf(" hr = %08x\n", (unsigned)hr);
	delete[] textureData;
	asyncArgBypass = nullptr;
}

}
