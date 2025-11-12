#pragma once
#include <cstdint>
#include <d3d11.h>
#include "cdcRender/Types.h"

namespace cdc {

class PCDX11RenderDevice;
class PCDX11StateManager;

class PCDX11BaseTexture {
protected:
	friend class PCDX11StateManager;
	uint16_t wrapMode;
	TextureClass shape; // 8
	uint32_t maxFilter; // C
	uint32_t textureFormat; // 10
public:
	uint32_t width; // 14
	uint32_t height; // 18
protected:
	uint32_t depth; // 1C
	PCDX11RenderDevice *renderDevice; // 20
	uint16_t mipLevels; // 24
public:

	static uint32_t StaticGetMemPitch(DXGI_FORMAT format, uint32_t width) {
		switch (format) {
			case DXGI_FORMAT_BC1_UNORM:
				return 8 * ((width + 3) >> 2);
			case DXGI_FORMAT_BC2_UNORM:
			case DXGI_FORMAT_BC3_UNORM:
				return 16 * ((width + 3) >> 2);
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				return 4 * width;
			case DXGI_FORMAT_A8_UNORM:
				return width;
			default:
				// printf("pitchForWidthAndFormat: unknown format %x\n", format);
				return format; // undefined behavior in the original game probably
		}
	}

	static uint32_t StaticGetLevelSize(DXGI_FORMAT fmt, uint32_t width, uint32_t height) {
		switch (fmt) {
			case DXGI_FORMAT_BC1_UNORM: // DXT1
				return 8 * ((width + 3) >> 2) * ((height + 3) >> 2);
			case DXGI_FORMAT_BC2_UNORM: // DXT4
			case DXGI_FORMAT_BC3_UNORM: // DXT5
				return 16 * ((width + 3) >> 2) * ((height + 3) >> 2);
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				return 4 * width * height;
			case DXGI_FORMAT_A8_UNORM:
				return width * height;
			default:
				//__builtin_unreachable();
				return 0;
		}
	}

	static uint32_t StaticGetByteSize(int width, int height, int depthMinusOne, int mipLevels, DXGI_FORMAT textureFormat) {
		uint32_t total = 0;
		uint32_t depth = depthMinusOne + 1;
		while (mipLevels--) {
			total += StaticGetLevelSize(textureFormat, width, height) * depth;
			if (width > 1) width >>= 1;
			if (height > 1) height >>= 1;
			if (depth > 1) depth >>= 1;
		}
		return total;
	}

	PCDX11BaseTexture(PCDX11RenderDevice *renderDevice, uint32_t dxgiFormat, uint16_t width, uint16_t height, /*...,*/ uint32_t maxFilter, uint16_t wrapMode, TextureClass shape) :
		wrapMode(wrapMode),
		shape(shape),
		maxFilter(maxFilter),
		textureFormat(dxgiFormat),
		width(width),
		height(height),
		renderDevice(renderDevice)
	{} // TODO: incomplete

	virtual ID3D11Resource *getTextureResource() = 0;
	virtual ID3D11ShaderResourceView *createShaderResourceView() = 0;
	virtual ID3D11UnorderedAccessView *createUnorderedAccessView() = 0;
	virtual bool baseTexMethodC() { return false; };
	virtual void baseTexMethod10() {};
};

}
