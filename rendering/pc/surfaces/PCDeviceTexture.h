#pragma once
#include <windows.h>
#include <d3d9types.h>
#include "PCDeviceBaseTexture.h"

struct IDirect3DBaseTexture9;
struct IDirect3DSurface9;
struct IDirect3DTexture9;

namespace cdc {

class PCDeviceTexture : public PCDeviceBaseTexture {

	uint32_t m_flags; // 10
	uint32_t m_width; // 14
	uint32_t m_height; // 18
	D3DFORMAT m_srcFormat; // 1C
	D3DFORMAT m_format; // 20
	uint32_t m_numMipMaps; // 24
	float m_mipMapLodBias; // 28
	Type m_type; // 2C
	// PCTextureUpdateCallback *m_pUpdateCallback; // 30
	void *m_pTextureData; // 34
	IDirect3DSurface9 *m_pD3DSurface; // 38
	IDirect3DTexture9 *m_pD3DTexture; // 3C

public:
	PCDeviceTexture(
		uint32_t width,
		uint32_t height,
		D3DFORMAT format,
		uint32_t numMipMaps,
		Type type,
		TextureFilter filter);

	// for PCInternalResource
	bool internalCreate() override;
	void internalRelease() override;

	// for PCDeviceBaseTexture
	void Release() override;
	IDirect3DBaseTexture9 *GetD3DTexture() override;
	void Upload(void *data) override;
	void UploadPartial(void *data, uint32_t offset, uint32_t length) override;
	float GetMipMapLodBias() override;
	void SetFilter(TextureFilter filter) override;
	uint32_t GetWidth() override;
	uint32_t GetHeight() override;
};

}
