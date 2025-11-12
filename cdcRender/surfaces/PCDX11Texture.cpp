#include <d3d11_1.h>
#include <cstdio>
#include <cstdlib>
#include "../PCDX11DeviceManager.h"
#include "../PCDX11Shared.h"
#include "PCDX11Texture.h"

namespace cdc {

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
	}
}

char *PCDX11Texture::resGetBuffer() {
	return (char*)textureBlob;
}

void PCDX11Texture::resConstruct() { /* empty */ }

void PCDX11Texture::resMethod10() { /* empty */ }

PCDX11Texture::~PCDX11Texture() {
	// TODO
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

	uint32_t format = ConvertFormatD3D9ToDXGI(textureBlob->format);

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
			uint32_t size = PCDX11BaseTexture::StaticGetLevelSize(textureDesc.Format, w, h);
			*k++ = {
				.pSysMem = l,
				.SysMemPitch = PCDX11BaseTexture::StaticGetMemPitch(textureDesc.Format, w),
				.SysMemSlicePitch = size
			};
			l += size;
		}

	auto device = deviceManager->getD3DDevice();
	HRESULT hr = device->CreateTexture2D(&hackTextureDesc, textureData, &d3dTexture128);
	printf(" texture %p\n", d3dTexture128);
	printf(" hr = %08x\n", (unsigned)hr);
	delete[] textureData;
	free(textureBlob);
}

}
