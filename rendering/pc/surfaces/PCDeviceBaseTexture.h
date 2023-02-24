#pragma once
#include <cstdint>
#include "../../Types.h"
#include "../PCInternalResource.h"

struct IDirect3DBaseTexture9;

namespace cdc {

class PCDeviceBaseTexture : public PCInternalResource {
protected:
	TextureFilter m_filter; // C

public:
	enum Type {
		kStatic = 0,
		kRenderTarget = 1,
		kDepthSurface = 2,
		kDynamic = 3
	};

	static void CreateFormatMappings();
	static void DestroyFormatMappings();

	bool internalCreate() override { // 4
		CreateFormatMappings();
		return true;
	}

	void internalRelease() override { // 8
		DestroyFormatMappings();
	}

	virtual void Release() = 0; // 18
	virtual IDirect3DBaseTexture9 *GetD3DTexture() = 0; // 1C
	virtual void Upload(void *data) = 0; // 20
	virtual void UploadPartial(void *data, uint32_t offset, uint32_t length) = 0; // 24
	virtual float GetMipMapLodBias() { return 0.0f; } // 28
	virtual void SetFilter(TextureFilter filter) { m_filter = filter; } // 2C
	virtual uint32_t GetWidth() = 0; // 30
	virtual uint32_t GetHeight() = 0; // 34
};

}
