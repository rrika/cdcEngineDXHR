#pragma once
#include "PCDX11BaseTexture.h"
#include "../PCDX11AsyncCreateResource.h"

struct ID3D11Texture2D;

namespace cdc {

class PCDX11RenderDevice;

class PCDX11BitmapTexture :
	public PCDX11BaseTexture,
	// public PCDX11DeferredUpdate,
	public PCDX11AsyncCreateResource
{
public:
	PCDX11BitmapTexture(
		PCDX11RenderDevice *renderDevice,
		uint32_t format, uint32_t b, uint32_t c, uint32_t d)
	:
		PCDX11BaseTexture(renderDevice, format, 0, 0, /*maxFilter=*/ 256, /*wrapMode=*/ 7, kTextureClassUnknown),
		PCDX11AsyncCreateResource()
	{
		// wrapMode = 7; // wrap in all axes (rather than clamp)
	}

	ID3D11Resource *getTextureResource() override;
	ID3D11ShaderResourceView *createShaderResourceView() override;
	ID3D11UnorderedAccessView *createUnorderedAccessView() override;

	void asyncCreate() override;

	uint8_t byte3C[4];
	uint8_t byte40[220];
	void  *imageData;
	uint32_t dword120;
	uint8_t byte124;
	ID3D11Texture2D *d3dTexture128 = nullptr;
	ID3D11ShaderResourceView *d3dResourceView12C = nullptr;
	ID3D11UnorderedAccessView *d3dAccessView130 = nullptr;
};

}
