#include <d3d11_1.h>
#include <stdio.h>
#include <stdlib.h>
#include "../PCDX11DeviceManager.h"
#include "PCDX11Texture.h"

namespace cdc {

static uint32_t bytesForTextureDim(TextureFormat fmt, uint32_t width, uint32_t height) {
	switch (fmt) {
		case TextureFormat::DXT1:
			return 8 * ((width + 3) >> 2) * ((height + 3) >> 2);
		case TextureFormat::DXT4:
		case TextureFormat::DXT5:
			return 16 * ((width + 3) >> 2) * ((height + 3) >> 2);
		case TextureFormat::Raw1Maybe:
		case TextureFormat::Raw2Maybe:
			return 4 * width * height;
		case TextureFormat::GrayScaleMaybe:
			return width * height;
	}
	//__builtin_unreachable();
	return 0;
}

static uint32_t bytesForTextureDimMips(int width, int height, int depthMinusOne, int mipLevels, TextureFormat textureFormat) {
	uint32_t total = 0;
	uint32_t depth = depthMinusOne + 1;
	while (mipLevels--) {
		total += bytesForTextureDim(textureFormat, width, height) * depth;
		if (width > 1) width >>= 1;
		if (height > 1) height >>= 1;
		if (depth > 1) depth >>= 1;
	}
	return total;
}

uint32_t decodeFormat(uint32_t format) {
	switch (format) {
		case '1TXD':
			return 71;
		case '2TXD':
			return 71;
		case '3TXD':
			return 74;
		case '4TXD':
			return 74;
		case '5TXD':
			return 77;
		case 20:
		case 21:
		case 22:
		case 26:
		case 27:
		case 29:
		case 30:
			return 28;
		case 23:
			return 85;
		case 24:
		case 25:
			return 86;
		case 28:
			return 65;
		case 31:
		case 35:
			return 24;
		case 32:
		case 33:
			return 87;
		case 34:
			return 35;
		case 36:
			return 11;
		case 111:
			return 54;
		case 112:
			return 34;
		case 113:
			return 10;
		case 114:
			return 41;
		case 115:
			return 16;
		case 116:
			return 2;
		default:
			return 0;
	}
}


uint32_t PCDX11Texture::getWidth() {
	return width;
}

uint32_t PCDX11Texture::getHeight() {
	return height;
}


void PCDX11Texture::resRegisterAtRenderDevice() {
	// TODO
}

void PCDX11Texture::resFill(void* src, size_t size, size_t offset) {
	if (PCDX11AsyncCreateResource::m_requested)
		return;

	// if (textureBlob == nullptr)
	// 	textureBlob = (TextureBlob*)malloc(sizeof(TextureBlob)); // TODO: pick alloc function

	// hack
	textureBlob = (TextureBlob*)malloc(size);
	memcpy(textureBlob, src, size);
}

char *PCDX11Texture::resGetBuffer() {
	return (char*)textureBlob;
}

void PCDX11Texture::resConstruct() { /* empty */ }

void PCDX11Texture::resMethod10() { /* empty */ }

void PCDX11Texture::asyncCreate(/* TODO: one argument */) {
	/*
		0000: cdc::PCDX11Texture
		0000:     cdc::TextureMap
		0000:         cdc::RenderResource
		0004:     cdc::PCDX11BitmapTexture
		0004:         cdc::PCDX11BaseTexture
		002C:         cdc::PCDX11DeferredUpdate
		0034:         cdc::PCDX11AsyncCreateResource <-- you are here
		0138:     cdc::PCDX11RenderExternalResource
		0138:         cdc::RenderExternalResource
	*/

	// hack implementation

	printf("async create texture %d x %d fmt=%08x (%d)\n", textureBlob->width, textureBlob->height,
		textureBlob->format, decodeFormat(textureBlob->format));

	// D3D11_TEXTURE2D_DESC textureDesc = {};
	D3D11_TEXTURE2D_DESC& textureDesc = hackTextureDesc;
	textureDesc = D3D11_TEXTURE2D_DESC {};
	textureDesc.Width              = textureBlob->width;
	textureDesc.Height             = textureBlob->height;
	textureDesc.MipLevels          = 1;
	textureDesc.ArraySize          = 1;
	textureDesc.Format             = DXGI_FORMAT_BC1_UNORM; //(DXGI_FORMAT)decodeFormat(textureBlob->format);
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.Usage              = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;

	// D3D11_SUBRESOURCE_DATA textureData = {};
	D3D11_SUBRESOURCE_DATA& textureData = hackTextureData;
	textureData = D3D11_SUBRESOURCE_DATA{};
	textureData.pSysMem            = (void*)(((char*)textureBlob)+0x1C);
	textureData.SysMemPitch        = textureBlob->width * 2;

	auto device = deviceManager->getD3DDevice();
	HRESULT hr = device->CreateTexture2D(&hackTextureDesc, &hackTextureData, &d3dTexture128);
	printf(" texture %p\n", d3dTexture128);
	printf(" hr = %08x\n", (unsigned)hr);
}

}
