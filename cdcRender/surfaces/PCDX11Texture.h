#pragma once
#include "../PCDX11RenderExternalResource.h"
#include "PCDX11BitmapTexture.h"
#include "TextureMap.h"
#include <d3d11_1.h>

namespace cdc {

class PCDX11RenderDevice;

uint32_t decodeFormat(uint32_t format);

struct TextureBlob {
	uint32_t magic;
	uint32_t format;
	uint32_t size;
	uint32_t unknownC;
	uint16_t width;
	uint16_t height;
	uint16_t volumeDepth;
	uint8_t depth;
	uint8_t mipLevels;
	uint16_t flags; // 18, wrap mode, etc.
	uint8_t textureClass; // 1A
};

class PCDX11Texture :
	public TextureMap,
	public PCDX11BitmapTexture,
	public PCDX11RenderExternalResource
{
public:
	uint32_t width; // 18
	uint32_t height; // 1C
	uint32_t dword144 = 0;
	TextureBlob *textureBlob = nullptr;
	uint32_t dword14C = 0;
	uint8_t numSlices = 0; // 6 for cubemap, 1 for normal texture
	char **m_pTextureData = nullptr;
	uint32_t *m_pLevelSizes = nullptr; // 158
	uint32_t m_textureSize = 0; // 15C, sum of m_pLevelSizes
	uint32_t dword160 = 0;
	uint32_t dword164 = 0;
	uint8_t byte168 = 0;

	D3D11_TEXTURE2D_DESC hackTextureDesc;

public:
	PCDX11Texture(PCDX11RenderDevice *renderDevice) :
		TextureMap(),
		PCDX11BitmapTexture(renderDevice, /*format=*/0, /*wrapMode=*/0, /*maxFilter=*/256, kTextureClassUnknown),
		PCDX11RenderExternalResource(renderDevice)
	{}

	// for RenderResource
	void resFree() override;
	void resFill(void* src, uint32_t size, uint32_t offset) override;
	char *resGetBuffer() override;
	void resConstruct() override;
	void resMethod10() override;
	~PCDX11Texture();

	// for TextureMap
	uint32_t getWidth() override;
	uint32_t getHeight() override;
	// void method_20() override;
	// void method_24() override;
	// void method_28() override;
	// void method_2C() override;

	// for PCDX11AsyncCreateResource
	void asyncCreate() override;
};

}
