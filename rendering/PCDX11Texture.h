#pragma once
#include "TextureMap.h"
#include "PCDX11BitmapTexture.h"
// #include "PCDX11RenderExternalResource.h"
#include <d3d11_1.h>

namespace cdc {

class PCDX11RenderDevice;

enum class TextureFormat { // DXGI_FORMAT ?
	DXT1 = 0x47, // 71
	DXT2 = 0x47, // 71
	DXT3 = 0x48, // 72
	DXT4 = 0x4A, // 74
	DXT5 = 0x4D, // 77
	Raw1Maybe = 0x1C,
	Raw2Maybe = 0x57,
	GrayScaleMaybe = 0x41,
};

struct TextureBlob {
	uint32_t magic;
	uint32_t format;
	uint32_t size;
	uint32_t unknownC;
	uint16_t width;
	uint16_t height;
	uint16_t bpp;
	uint8_t unknown16;
	uint8_t mipLevels;
	uint16_t flags;
	uint16_t unknown1A;
};

class PCDX11Texture :
	public TextureMap,
	public PCDX11BitmapTexture
	// public PCDX11RenderExternalResource
{
public:
	uint32_t dword144 = 0;
	TextureBlob *textureBlob = nullptr;
	uint32_t dword14C = 0;
	uint8_t numSlices = 0; // 6 for cubemap, 1 for normal texture
	void **imageData = nullptr;
	uint32_t *perMipSize = nullptr;
	uint32_t mipChainSize = 0;
	uint32_t dword160 = 0;
	uint32_t dword164 = 0;
	uint8_t byte168 = 0;

	D3D11_TEXTURE2D_DESC hackTextureDesc;
	D3D11_SUBRESOURCE_DATA hackTextureData;

public:
	PCDX11Texture(PCDX11RenderDevice *renderDevice) :
		TextureMap(),
		PCDX11BitmapTexture(renderDevice, 0, 0, 256, 0)
		// PCDX11RenderExternalResource(renderDevice)
	{}

	// for RenderResource
	void resRegisterAtRenderDevice() override;
	void resFill(void* src, size_t size, size_t offset) override;
	char *resGetBuffer() override;
	void resConstruct() override;
	void resMethod10() override;

	// for PCDX11AsyncCreateResource
	void asyncCreate() override;
};

}
