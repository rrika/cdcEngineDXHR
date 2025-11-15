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


	if (offset + size + 4 >= textureBlob->size) {

		PCDX11BaseTexture::shape = (TextureClass)textureBlob->textureClass;
		uint32_t format = ConvertFormatD3D9ToDXGI(textureBlob->format);

		// ignore request for RGBA, always do BGRA
		if (format == 28) // DXGI_FORMAT_R8G8B8A8_UNORM
			format = 87; // DXGI_FORMAT_B8G8R8A8_UNORM
		PCDX11BaseTexture::textureFormat = (DXGI_FORMAT)format;

		if (!(textureBlob->flags & 0x4000)) // not volume
			textureBlob->volumeDepth = 0;

		textureBlob->mipLevels++;

		if (textureBlob->flags & 0x8000) // cubemap
			numSlices = 6;
		else
			numSlices = 1;

		char **slice = m_pTextureData = new char*[
			numSlices *
			textureBlob->mipLevels];

		char *l = 0x1C + (char*)textureBlob;
		for (auto i=0; i < numSlices; i++)
			for (auto j=0; j < textureBlob->mipLevels; j++) {
				uint32_t w = textureBlob->width >> j;
				uint32_t h = textureBlob->height >> j;
				uint32_t d = (textureBlob->volumeDepth >> j) + 1;
				uint32_t size = d * PCDX11BaseTexture::StaticGetLevelSize((DXGI_FORMAT)format, w, h);
				*slice++ = l;
				l += size;
			}

		// TODO

		CreateD3DTexture(
			(DXGI_FORMAT)format,
			textureBlob->width,
			textureBlob->height,
			textureBlob->volumeDepth,
			numSlices,
			textureBlob->mipLevels,
			(uint8_t**)m_pTextureData,
			textureBlob->flags & 7,
			nullptr);
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

void PCDX11Texture::asyncCreate() {
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

	PCDX11BitmapTexture::asyncCreate();
	delete[] m_pTextureData;
	free(textureBlob);
}

}
