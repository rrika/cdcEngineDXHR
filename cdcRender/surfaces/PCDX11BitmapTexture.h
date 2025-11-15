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
		uint32_t format, uint16_t wrapMode, uint32_t maxFilter, TextureClass shape)
	:
		PCDX11BaseTexture(renderDevice, format, 0, 0, maxFilter, wrapMode, shape),
		PCDX11AsyncCreateResource()
	{
	}
	~PCDX11BitmapTexture();
	void CreateD3DTexture(
		DXGI_FORMAT format,
		uint16_t width,
		uint16_t height,
		uint16_t depth,
		uint16_t arraySize,
		uint8_t mipLevels,
		uint8_t **ppData,
		uint16_t wrapmode,
		const char *pName);

	ID3D11Resource *getTextureResource() override;
	ID3D11ShaderResourceView *createShaderResourceView() override;
	ID3D11UnorderedAccessView *createUnorderedAccessView() override;

	void asyncCreate() override;

	uint8_t byte3C[4];
	uint8_t byte40[220];
	void *textureMemory; // 11C
	uint32_t arraySize; // 120
	uint8_t byte124;
	ID3D11Texture2D *d3dTexture128 = nullptr;
	ID3D11ShaderResourceView *d3dResourceView12C = nullptr;
	ID3D11UnorderedAccessView *d3dAccessView130 = nullptr;

	char **asyncArgBypass = nullptr; // HACK
};

}
