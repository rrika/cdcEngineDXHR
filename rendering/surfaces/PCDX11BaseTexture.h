#pragma once
#include <cstdint>
#include <d3d11.h>
#include "rendering/Types.h"

namespace cdc {

class PCDX11RenderDevice;
class PCDX11StateManager;

class PCDX11BaseTexture {
protected:
	friend class PCDX11StateManager;
	uint16_t repeatmode : 3;
	uint16_t unknownflags : 13;
	TextureClass shape; // 8
	uint32_t maxFilter; // C
	uint32_t textureFormat; // 10
	uint32_t width; // 14
	uint32_t height; // 18
	uint32_t bpp; // 1C
	PCDX11RenderDevice *renderDevice; // 20
	uint16_t mipLevels; // 24
public:
	PCDX11BaseTexture(uint32_t width, uint32_t height, uint32_t dxgiFormat) :
		width(width),
		height(height),
		textureFormat(dxgiFormat)
	{} // TODO: incomplete

	virtual ID3D11Resource *getTextureResource() = 0;
	virtual ID3D11ShaderResourceView *createShaderResourceView() = 0;
	virtual ID3D11UnorderedAccessView *createUnorderedAccessView() = 0;
	virtual bool baseTexMethodC() { return false; };
	virtual void baseTexMethod10() {};
};

}
