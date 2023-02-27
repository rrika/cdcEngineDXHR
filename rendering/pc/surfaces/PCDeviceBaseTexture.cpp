#include <cstdio>
#include <windows.h>
#include <d3d9.h>
#include "cdcSys/Assert.h"
#include "../PCDeviceManager.h"
#include "PCDeviceBaseTexture.h"

namespace cdc {

static const D3DFORMAT kINTZFormat = (D3DFORMAT) 0x5A544E49;
static const D3DFORMAT kDF24Format = (D3DFORMAT) 0x34324644;
static const D3DFORMAT kDF16Format = (D3DFORMAT) 0x36314644;

uint32_t PCDeviceBaseTexture::s_formatRefCount = 0;

PCDeviceBaseTexture::FormatInfo PCDeviceBaseTexture::s_formats[27] = {
	{D3DFMT_R5G6B5,   /*mask*/ 0x00000000, 0x0000F800, 0x000007E0, 0x0000001F, /*shift*/ 0x00, 0x0B, 0x05, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 2, 0},
	{D3DFMT_X1R5G5B5, /*mask*/ 0x00000000, 0x00007C00, 0x000003E0, 0x0000001F, /*shift*/ 0x0F, 0x0A, 0x05, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 2, 0},
	{D3DFMT_A1R5G5B5, /*mask*/ 0x00008000, 0x00007C00, 0x000003E0, 0x0000001F, /*shift*/ 0x0F, 0x0A, 0x05, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 2, 0},
	{D3DFMT_A4R4G4B4, /*mask*/ 0x0000F000, 0x00000F00, 0x000000F0, 0x0000000F, /*shift*/ 0x0C, 0x08, 0x04, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 2, 0},
	{D3DFMT_X8R8G8B8, /*mask*/ 0x00000000, 0x00FF0000, 0x0000FF00, 0x000000FF, /*shift*/ 0x18, 0x10, 0x08, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_A8R8G8B8, /*mask*/ 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF, /*shift*/ 0x18, 0x10, 0x08, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_DXT1,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 2, 0},
	{D3DFMT_DXT2,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_DXT3,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_DXT4,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_DXT5,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_R16F,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 2, 0},
	{D3DFMT_G16R16F,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_R32F,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_G16R16,   /*mask*/ 0x00000000, 0x0000FFFF, 0xFFFF0000, 0x00000000, /*shift*/ 0x00, 0x00, 0x10, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_A8,       /*mask*/ 0x000000FF, 0x00000000, 0x00000000, 0x00000000, /*shift*/ 0x00, 0x00, 0x00, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 1, 0},
	{D3DFMT_A8L8,     /*mask*/ 0x0000FF00, 0x000000FF, 0x000000FF, 0x000000FF, /*shift*/ 0x08, 0x00, 0x00, 0, /*mul*/ 0, 0, 0, 0, /*bpp*/ 2, 0},
	{D3DFMT_D24X8,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_D24S8,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_D24X4S4,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_D16,      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 2, 0},
	{D3DFMT_D32,      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_D24FS8,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{kINTZFormat,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{kDF24Format,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{kDF16Format,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 4, 0},
	{D3DFMT_UNKNOWN,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*bpp*/ 0, 0}
};


uint32_t PCDeviceBaseTexture::GetLevelSize(D3DFORMAT format, uint32_t w, uint32_t h) { // line 31
	switch (format) {
		case D3DFMT_DXT1:
			return 8 * ((w + 3) >> 2) * ((h + 3) >> 2);
		case D3DFMT_DXT2:
		case D3DFMT_DXT3:
		case D3DFMT_DXT4:
		case D3DFMT_DXT5:
			return 16 * ((w + 3) >> 2) * ((h + 3) >> 2);
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
			return 4 * w * h;
		case D3DFMT_R5G6B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_A4R4G4B4:
		case D3DFMT_A8L8:
			return 2 * w * h;
		case D3DFMT_A8:
			return w * h;
		default:
			FatalError("a");
			return 0;
	}
}

uint32_t PCDeviceBaseTexture::GetByteSize( // line 70, see also PCDX11BaseTexture::StaticGetByteSize
	uint32_t width,
	uint32_t height,
	uint32_t depth,
	uint32_t numMipMaps,
	D3DFORMAT format
) {
	uint32_t byteSize = 0;
	while (numMipMaps--) {
		byteSize += depth * GetLevelSize(format, width, height);
		if (width > 1) width >>= 1;
		if (height > 1) height >>= 1;
		if (depth > 1) depth >>= 1;
	}
	return byteSize;
}

void PCDeviceBaseTexture::UpdateD3DSurfaceLevel( // line 87
	void *srcData,
	uint32_t byteOffset,
	uint32_t numBytes,
	uint32_t xsize,
	uint32_t ysize,
	uint32_t zsize,
	uint32_t mipmapLevel,
	uint32_t faceIdx,
	IDirect3DBaseTexture9 *pBaseTexture,
	D3DFORMAT format
) {
	IDirect3DTexture9 *pRegTex = nullptr;
	IDirect3DCubeTexture9 *pCubeTex = nullptr;
	IDirect3DVolumeTexture9 *pVolTex = nullptr;

	HRESULT hr;
	D3DLOCKED_RECT locked_rect;

	printf("      UpdateD3DSurfaceLevel\n");

	if (pBaseTexture->GetType() == D3DRTYPE_CUBETEXTURE) {
		// TODO
	} else if (pBaseTexture->GetType() == D3DRTYPE_VOLUMETEXTURE) {
		// TODO
	} else {
		pRegTex = static_cast<IDirect3DTexture9*>(pBaseTexture);
		hr = pRegTex->LockRect(
			mipmapLevel,
			&locked_rect,
			/*rect=*/ nullptr,
			/*flags=*/ 0);
	}
	if (hr < 0)
		return;

	FormatInfo *srcFmt = GetFormatInfo(format);
	FormatInfo *dstFmt = GetDestFormatInfo(format, 1 /*HACK*/);

	if (srcData) {

		void *dstData = locked_rect.pBits;

		if (srcFmt == dstFmt) {
			// if (FormatIsDXTC(srcFmt->format))
			memcpy((char*)dstData + byteOffset, (char*)srcData + byteOffset, numBytes);

		} else {
			FatalError("UpdateD3DSurfaceLevel: unimplemented srcFmt != dstFmt");
		}
	} else {
		FatalError("UpdateD3DSurfaceLevel: unimplemented srcData == nullptr");
	}

	if (pRegTex)
		pRegTex->UnlockRect(mipmapLevel);
}

PCDeviceBaseTexture::FormatInfo *PCDeviceBaseTexture::GetFormatInfo(D3DFORMAT format) { // line 512
	FormatInfo *info = s_formats;
	while (info->format != format)
		info++;
	return info;
}

PCDeviceBaseTexture::FormatInfo *PCDeviceBaseTexture::GetDestFormatInfo(D3DFORMAT src, uint8_t usageFlags) { // line 522
	FormatInfo *fmt = GetFormatInfo(src);
	if (fmt->pDest && (fmt->destFlags & usageFlags))
		return fmt->pDest;
	return fmt;
}

uint8_t PCDeviceBaseTexture::CheckFormat( // line 530
	FormatInfo *pFormatInfo,
	uint8_t flags
) {
	uint8_t supported = 0;
	if (deviceManager9->CheckFormat(pFormatInfo->format, D3DRTYPE_TEXTURE, 0))
		supported |= 1;
	if (deviceManager9->CheckFormat(pFormatInfo->format, D3DRTYPE_CUBETEXTURE, 0))
		supported |= 2;
	if (deviceManager9->CheckFormat(pFormatInfo->format, D3DRTYPE_VOLUMETEXTURE, 0))
		supported |= 4;
	return flags & supported;
}

void PCDeviceBaseTexture::CreateFormatMapping( // line 548
	FormatInfo *pSrc,
	FormatInfo *pDest,
	uint8_t destFlags
) {
	if (pSrc != pDest) {
		pSrc->destFlags = destFlags;
		pSrc->pDest = pDest;
		if (!FormatIsDXTC(pDest->format)) {
			// TODO
		}
	}
}

void PCDeviceBaseTexture::CreateFormatMappings() { // line 579
	if (s_formatRefCount++ > 0)
		return;

	FormatInfo *pSrc, *pDest;
	uint8_t flags, destFlags;

	// instead of 0565 try 1555, 8888, 4444

	flags = CheckFormat(pSrc = pDest = GetFormatInfo(D3DFMT_R5G6B5), destFlags = 7);
	if (flags != destFlags) {
		destFlags ^= flags;
		if (CheckFormat(pDest = GetFormatInfo(D3DFMT_A1R5G5B5), destFlags) == destFlags)
			;
		else if (CheckFormat(pDest = GetFormatInfo(D3DFMT_A8R8G8B8), destFlags) == destFlags)
			;
		else if (CheckFormat(pDest = GetFormatInfo(D3DFMT_A4R4G4B4), destFlags) == destFlags)
			;
		else
			FatalError("No texture-format available to pick for 0565 input!");
	}
	CreateFormatMapping(pSrc, pDest, destFlags);

	// instead of 1555 try 8888, 4444

	flags = CheckFormat(pSrc = pDest = GetFormatInfo(D3DFMT_A1R5G5B5), destFlags = 7);
	if (flags != destFlags) {
		destFlags ^= flags;
		if (CheckFormat(pDest = GetFormatInfo(D3DFMT_A8R8G8B8), destFlags) == destFlags)
			;
		else if (CheckFormat(pDest = GetFormatInfo(D3DFMT_A4R4G4B4), destFlags) == destFlags)
			;
		else
			FatalError("No texture-format available to pick for 1555 input!");
	}
	CreateFormatMapping(pSrc, pDest, destFlags);

	// instead of 4444 try 8888

	flags = CheckFormat(pSrc = pDest = GetFormatInfo(D3DFMT_A4R4G4B4), destFlags = 7);
	if (flags != destFlags) {
		destFlags ^= flags;
		if (CheckFormat(pDest = GetFormatInfo(D3DFMT_A8R8G8B8), destFlags) == destFlags)
			;
		else
			FatalError("No texture-format available to pick for 4444 input!");
	}
	CreateFormatMapping(pSrc, pDest, destFlags);

	// instead of 8888 try 4444

	flags = CheckFormat(pSrc = pDest = GetFormatInfo(D3DFMT_A8R8G8B8), destFlags = 7);
	if (flags != destFlags) {
		destFlags ^= flags;
		if (CheckFormat(pDest = GetFormatInfo(D3DFMT_A4R4G4B4), destFlags) == destFlags)
			;
		else
			FatalError("No texture-format available to pick for 8888 input!");
	}
	CreateFormatMapping(pSrc, pDest, destFlags);

	// TODO

	// HACK
	pSrc = pDest = GetFormatInfo(D3DFMT_DXT1); CreateFormatMapping(pSrc, pDest, 7);
	pSrc = pDest = GetFormatInfo(D3DFMT_DXT2); CreateFormatMapping(pSrc, pDest, 7);
	pSrc = pDest = GetFormatInfo(D3DFMT_DXT3); CreateFormatMapping(pSrc, pDest, 7);
	pSrc = pDest = GetFormatInfo(D3DFMT_DXT4); CreateFormatMapping(pSrc, pDest, 7);
	pSrc = pDest = GetFormatInfo(D3DFMT_DXT5); CreateFormatMapping(pSrc, pDest, 7);
}

void PCDeviceBaseTexture::DestroyFormatMappings() { // line 720
	if (--s_formatRefCount > 0)
		return;

	// TODO
}

void *PCDeviceBaseTexture::TextureDataAlloc(uint32_t numBytes) { // line 736
	#ifdef _WIN32
		return VirtualAlloc(0, numBytes, MEM_COMMIT, PAGE_READWRITE);
	#else
		return (void*) new char[numBytes];
	#endif
}

void PCDeviceBaseTexture::TextureDataFree(void *pAddr) { // line 746
	#ifdef _WIN32
		VirtualFree(pAddr, 0, MEM_RELEASE);
	#else
		delete[] (char*) pAddr;
	#endif
}

}