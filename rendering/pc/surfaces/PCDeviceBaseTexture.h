#pragma once
#include <cstdint>
#include "../../Types.h"
#include "../PCInternalResource.h"

struct IDirect3DBaseTexture9;

namespace cdc {

class PCDeviceBaseTexture : public PCInternalResource {
public:
	struct FormatInfo {
		D3DFORMAT format;
		uint32_t aMask;
		uint32_t rMask;
		uint32_t gMask;
		uint32_t bMask;
		uint8_t aOff;
		uint8_t rOff;
		uint8_t gOff;
		uint8_t bOff;
		int aMul;
		int rMul;
		int gMul;
		int bMul;
		uint8_t bpp;
		uint8_t destFlags;
		FormatInfo *pDest;
	};

protected:
	static uint32_t s_formatRefCount;
	static FormatInfo s_formats[27];

	TextureFilter m_filter; // C

public:
	enum Type {
		kStatic = 0,
		kRenderTarget = 1,
		kDepthSurface = 2,
		kDynamic = 3
	};

	static uint32_t GetLevelSize(
		D3DFORMAT format,
		uint32_t w,
		uint32_t h);
	static uint32_t GetByteSize(
		uint32_t width,
		uint32_t height,
		uint32_t depth,
		uint32_t numMipMaps,
		D3DFORMAT format);
	static void UpdateD3DSurfaceLevel(
		void *srcData,
		uint32_t byteOffset,
		uint32_t numBytes,
		uint32_t xsize,
		uint32_t ysize,
		uint32_t zsize,
		uint32_t mipmapLevel,
		uint32_t faceIdx,
		IDirect3DBaseTexture9 *pBaseTexture,
		D3DFORMAT format);
	static FormatInfo *GetFormatInfo(D3DFORMAT format);
	static FormatInfo *GetDestFormatInfo(D3DFORMAT src, uint8_t usageFlags);
	static uint8_t CheckFormat(
		FormatInfo *pFormatInfo,
		uint8_t flags);
	static void CreateFormatMapping(
		FormatInfo *pSrc,
		FormatInfo *pDest,
		uint8_t destFlags);
	static void CreateFormatMappings();
	static void DestroyFormatMappings();
	static void *TextureDataAlloc(uint32_t numBytes);
	static void TextureDataFree(void *pAddr);
	static bool FormatIsDXTC(D3DFORMAT format) {
		return
			format == D3DFMT_DXT1 ||
			format == D3DFMT_DXT2 ||
			format == D3DFMT_DXT3 ||
			format == D3DFMT_DXT4 ||
			format == D3DFMT_DXT5;
	}

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
