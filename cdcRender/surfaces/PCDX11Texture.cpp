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
		default:
			//__builtin_unreachable();
			return 0;
	}
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

// cdc format to dxgi format
uint32_t decodeFormat(uint32_t format) {
	switch (format) {
		case '1TXD': // 0x31545844
			return 71; // DXGI_FORMAT_BC1_UNORM
		case '2TXD':
			return 71; // DXGI_FORMAT_BC1_UNORM
		case '3TXD':
			return 74; // DXGI_FORMAT_BC2_UNORM
		case '4TXD':
			return 74; // DXGI_FORMAT_BC2_UNORM
		case '5TXD':
			return 77; // DXGI_FORMAT_BC3_UNORM
		case 20:
		case 21:
		case 22:
		case 26:
		case 27:
		case 29:
		case 30:
			return 28; // DXGI_FORMAT_R8G8B8A8_UNORM
		case 23:
			return 85; // DXGI_FORMAT_B5G6R5_UNORM
		case 24:
		case 25:
			return 86; // DXGI_FORMAT_B5G5R5A1_UNORM
		case 28:
			return 65; // DXGI_FORMAT_A8_UNORM
		case 31:
		case 35:
			return 24; // DXGI_FORMAT_R10G10B10A2_UNORM
		case 32:
		case 33:
			return 87; // DXGI_FORMAT_B8G8R8A8_UNORM
		case 34:
			return 35; // DXGI_FORMAT_R16G16_UNORM
		case 36:
			return 11; // DXGI_FORMAT_R16G16B16A16_UNORM
		case 111:
			return 54; // DXGI_FORMAT_R16_FLOAT
		case 112:
			return 34; // DXGI_FORMAT_R16G16_FLOAT
		case 113:
			return 10; // DXGI_FORMAT_R16G16B16A16_FLOAT
		case 114:
			return 41; // DXGI_FORMAT_R32_FLOAT
		case 115:
			return 16; // DXGI_FORMAT_R32G32_FLOAT
		case 116:
			return 2; // DXGI_FORMAT_R32G32B32A32_FLOAT
		default:
			return 0; // DXGI_FORMAT_UNKNOWN
	}
}

uint32_t pitchForFormatAndWidth(uint32_t format, uint32_t width) {
	switch (format) {
		case 71:
			return 8 * ((width + 3) >> 2);
		case 74:
		case 77:
			return 16 * ((width + 3) >> 2);
		case 28:
		case 87:
			return 4 * width;
		case 65:
			return width;
		default:
			printf("pitchForWidthAndFormat: unknown format %x\n", format);
			return format; // undefined behavior in the original game probably
	}
}

uint32_t PCDX11Texture::getWidth() {
	return width;
}

uint32_t PCDX11Texture::getHeight() {
	return height;
}


void PCDX11Texture::resFree() {
	// HACK
	delete this;
}

void PCDX11Texture::resFill(void* src, uint32_t size, uint32_t offset) {
	if (PCDX11AsyncCreateResource::m_requested)
		return;

	// if (textureBlob == nullptr)
	// 	textureBlob = (TextureBlob*)malloc(sizeof(TextureBlob)); // TODO: pick alloc function

	// hack
	textureBlob = (TextureBlob*)malloc(size);
	memcpy(textureBlob, src, size);

	PCDX11BaseTexture::width     = textureBlob->width;
	PCDX11BaseTexture::height    = textureBlob->height;
	PCDX11BaseTexture::mipLevels = textureBlob->mipLevels + 1;

	if (offset + size + 4 >= textureBlob->size) {
		request(0);
		awaitResource();
	}
}

char *PCDX11Texture::resGetBuffer() {
	return (char*)textureBlob;
}

void PCDX11Texture::resConstruct() { /* empty */ }

void PCDX11Texture::resMethod10() { /* empty */ }

PCDX11Texture::~PCDX11Texture() {
	// TODO
	free(textureBlob);
}

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

	uint32_t format = decodeFormat(textureBlob->format);

	// ignore request for RGBA, always do BGRA
	if (format == 28) // DXGI_FORMAT_R8G8B8A8_UNORM
		format = 87; // DXGI_FORMAT_B8G8R8A8_UNORM

	printf("async create texture %d x %d fmt=%08x (%d)\n", textureBlob->width, textureBlob->height,
		textureBlob->format, format);

	this->wrapMode = textureBlob->flags & 7;
	this->shape = (TextureClass)textureBlob->textureClass;

	// D3D11_TEXTURE2D_DESC textureDesc = {};
	D3D11_TEXTURE2D_DESC& textureDesc = hackTextureDesc;
	textureDesc = D3D11_TEXTURE2D_DESC {};
	textureDesc.Width              = PCDX11BaseTexture::width;
	textureDesc.Height             = PCDX11BaseTexture::height;
	textureDesc.MipLevels          = PCDX11BaseTexture::mipLevels;
	textureDesc.ArraySize          = 1;
	textureDesc.Format             = (DXGI_FORMAT)format;
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.Usage              = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA *textureData = new D3D11_SUBRESOURCE_DATA[
		textureDesc.ArraySize *
		textureDesc.MipLevels];

	D3D11_SUBRESOURCE_DATA *k = textureData;
	char *l = 0x1C + (char*)textureBlob;
	for (auto i=0; i < textureDesc.ArraySize; i++)
		for (auto j=0; j < textureDesc.MipLevels; j++) {
			uint32_t w = textureBlob->width >> j;
			uint32_t h = textureBlob->height >> j;
			uint32_t size = bytesForTextureDim((TextureFormat)textureDesc.Format, w, h);
			*k++ = {
				.pSysMem = l,
				.SysMemPitch = pitchForFormatAndWidth(textureDesc.Format, w),
				.SysMemSlicePitch = size
			};
			l += size;
		}

	auto device = deviceManager->getD3DDevice();
	HRESULT hr = device->CreateTexture2D(&hackTextureDesc, textureData, &d3dTexture128);
	printf(" texture %p\n", d3dTexture128);
	printf(" hr = %08x\n", (unsigned)hr);
	delete[] textureData;
}

}
