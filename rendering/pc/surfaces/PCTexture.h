#pragma once
#include <windows.h> // for d3dtypes
#include <d3d9types.h> // for D3DFORMAT
#include "../../surfaces/TextureMap.h"
#include "../PCRenderDevice.h"
#include "../PCRenderExternalResource.h"
#include "PCBaseTexture.h"

namespace cdc {

class PCRenderDevice;

struct PCTextureData { // line 31, sizeof(PCTextureData) = 28, compare with TextureBlob

	// static const uint16_t kWrapModeClamp = ...;
	// static const uint16_t kWrapModeRepeatU = ...;
	// static const uint16_t kWrapModeRepeatV = ...;
	// static const uint16_t kWrapModeRepeatW = ...;
	// static const uint16_t kWrapModeRepeatUV = ...;
	// static const uint16_t kWrapModeRepeatUVW = ...;
	// static const uint16_t kWrapModeMask = ...;
	// static const uint16_t kNoQualityReduction = ...;
	// static const uint16_t kRLE32 = ...;
	static const uint16_t kResourceName = 0x2000; // guess
	static const uint16_t kVolumeMap = 0x4000;
	static const uint16_t kCubeMap = 0x8000;
	static const uint32_t kMagicNumber = 0x39444350; // 'PCD9'
	static const uint32_t kResourceNameSize = 256; // guess

	uint32_t magicNumber; // expect kMagicNumber
	D3DFORMAT format;
	uint32_t bitmapSize;
	uint32_t highResMipLevels;
	uint16_t width;
	uint16_t height;
	uint16_t volumeDepth;
	uint8_t depth;
	uint8_t numMipMaps;
	uint16_t flags;
	uint8_t m_class;
};

class PCTexture :
	public TextureMap, // 0
	public PCBaseTexture, // 4
	public PCRenderExternalResource // 14
{
public:
	PCTextureData *m_pResourceData = nullptr; // 20, header of texture data
	uint32_t m_nSkipBytes = 0;         // 24
	uint8_t *m_pTextureData = nullptr; // 28
	uint32_t m_offset = 0;             // 2C

public:
	PCTexture(PCRenderDevice *renderDevice) :
		TextureMap(),
		PCBaseTexture(nullptr, 0, 0),
		PCRenderExternalResource(renderDevice)
	{

	}

	// for RenderResource
	void resFree() override;
	void resFill(void *src, uint32_t size, uint32_t offset) override;
	char *resGetBuffer() override;
	void resConstruct() override;
	void resMethod10() override;

	// for TextureMap
	uint32_t getWidth() override;
	uint32_t getHeight() override;
	// void method_20() override;
	// void method_24() override;
	// void method_28() override;
	// void method_2C() override;

	// for PCBaseTexture
	// inherit bool NeedsUpdate();
	// inherit void Update();

	// for PCRenderExternalResource
	// auto-generate destructor
};

}
