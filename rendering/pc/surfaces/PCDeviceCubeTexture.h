#pragma once

namespace cdc {

class PCDeviceCubeTexture : public PCDeviceBaseTexture {
public:
	// for PCInternalResource
	bool internalCreate() override;
	void internalRelease() override;

	// for PCDeviceBaseTexture
	void Release() override;
	IDirect3DSurface9 *GetD3DTexture() override;
	void Upload(void *data) override;
	void UploadPartial(void *data, uint32_t offset, uint32_t length) override;
	float GetMipMapLodBias() override;
	void SetFilter(TextureFilter filter) override;
	uint32_t GetWidth() override;
	uint32_t GetHeight() override;
};

}
