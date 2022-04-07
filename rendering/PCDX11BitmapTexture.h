#pragma once
#include "PCDX11AsyncCreateResource.h"

struct ID3D11Texture2D;

namespace cdc {

class PCDX11RenderDevice;

class PCDX11BitmapTexture :
	// public PCDX11BaseTexture,
	// public PCDX11DeferredUpdate,
	public PCDX11AsyncCreateResource
{
public:
	PCDX11BitmapTexture(
		PCDX11RenderDevice *renderDevice,
		uint32_t a, uint32_t b, uint32_t c, uint32_t d)
	:
		PCDX11AsyncCreateResource()
	{}

	uint8_t byte3C[4];
	uint8_t byte40[220];
	void  *imageData;
	uint32_t dword120;
	uint8_t byte124;
	ID3D11Texture2D *d3dTexture128 = nullptr;
	uint32_t dword12C;
	uint32_t dword130;
};

}
