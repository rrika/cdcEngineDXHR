#pragma once
#include <cstdint>
#include <d3d11.h>

namespace cdc {

class PCDX11RenderDevice;

class PCDX11BaseTexture {
protected:
	uint16_t word4;
	uint32_t shape;
	uint32_t dwordC;
	uint32_t textureFormat;
	uint32_t width;
	uint32_t height;
	uint32_t bpp;
	PCDX11RenderDevice *renderDevice;
	uint16_t mipLevels;
public:
	virtual void baseTexWait() = 0;
	virtual ID3D11ShaderResourceView *createShaderResourceView() = 0;
	virtual ID3D11UnorderedAccessView *createUnorderedAccessView() = 0;
	virtual bool baseTexMethodC() { return false; };
	virtual void baseTexMethod10() {};
};

}
