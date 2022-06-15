#pragma once
#include <cstdint>
#include <d3d11.h>

namespace cdc {

class PCDX11RenderDevice;
class PCDX11StateManager;

class PCDX11BaseTexture {
protected:
	friend class PCDX11StateManager;
	uint16_t repeatmode : 3;
	uint16_t unknownflags : 13;
	uint32_t shape;
	uint32_t maxFilter;
	uint32_t textureFormat;
	uint32_t width;
	uint32_t height;
	uint32_t bpp;
	PCDX11RenderDevice *renderDevice;
	uint16_t mipLevels;
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
