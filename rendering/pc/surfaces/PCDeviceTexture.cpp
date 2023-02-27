#include <cstdio>
#include <d3d9.h>
#include "../PCDeviceManager.h"
#include "PCDeviceTexture.h"

namespace cdc {

PCDeviceTexture::PCDeviceTexture(
	uint32_t width,
	uint32_t height,
	D3DFORMAT format,
	uint32_t numMipMaps,
	PCDeviceBaseTexture::Type type,
	TextureFilter filter)
:
	m_flags(1),
	m_width(width),
	m_height(height),
	m_srcFormat(format),
	m_format(D3DFMT_UNKNOWN),
	m_numMipMaps(type != kStatic ? numMipMaps : 1), // unsure
	m_mipMapLodBias(0.0f),
	m_type(type),
	// m_pUpdateCallback(nullptr),
	m_pTextureData(nullptr),
	m_pD3DSurface(nullptr),
	m_pD3DTexture(nullptr)

{
	m_filter = filter;
	internalCreateIfDeviceManagerAgrees();
}

uint32_t PCDeviceTexture::GetByteSize() { // line 330
	return PCDeviceBaseTexture::GetByteSize(
		m_width,
		m_height,
		/*depth=*/ 1,
		m_numMipMaps,
		m_srcFormat
	);
}

void PCDeviceTexture::UpdateD3DTexture(void *voidData, uint32_t offset, uint32_t length) { // line 342

	uint32_t w = m_width;
	uint32_t h = m_height;
	uint32_t numlevels = m_pD3DTexture->GetLevelCount();
	uint32_t mipBegin = 0;
	char *data = (char*)voidData;

	printf("  PCDeviceTexture::UpdateD3DTexture %x+%x\n", offset, length);

	for (uint32_t i=0; i<numlevels; i++) {
		// uint32_t numMipBytes = GetByteSize(w, h, 1, 1, m_srcFormat);
		uint32_t numMipBytes = GetLevelSize(m_srcFormat, w, h);
		uint32_t intraMipOffset = offset - mipBegin;
		printf("    mip %d (%dx%d) %x+%x  ", i, w, h, mipBegin, numMipBytes);

		if (intraMipOffset < numMipBytes) {

			uint32_t updateAmount = length;
			if (updateAmount > numMipBytes - intraMipOffset)
				updateAmount = numMipBytes - intraMipOffset;

			if (updateAmount == 0)
				return;

			printf("copy %x+%x -> %x+%x\n", offset, updateAmount, offset-mipBegin, updateAmount);

			UpdateD3DSurfaceLevel(
				data,
				offset - mipBegin,
				updateAmount,
				w, h, 1,
				i, 0,
				m_pD3DTexture,
				m_srcFormat);

		} else
			printf("skip\n");

		if (data)
			data += numMipBytes;
		mipBegin += numMipBytes;
		if (w > 1)
			w >>= 1;
		if (h > 1)
			h >>= 1;
	}
}

bool PCDeviceTexture::internalCreate() {
	if (!PCDeviceBaseTexture::internalCreate())
		return false;

	bool isNonPow2 = (m_width & (m_width-1)) || (m_height & (m_height - 1));

	if (isNonPow2 && (deviceManager9->GetCaps() & PCDeviceManager::PC_CAPS_TEXTURE_NONPOW2) == 0) {
		// TODO
	}

	uint32_t levels = m_numMipMaps;

	if ((deviceManager9->GetCaps() & PCDeviceManager::PC_CAPS_TEXTURE_MIPMAPS) == 0)
		levels = 1;

	D3DPOOL pool = D3DPOOL_MANAGED;
	uint32_t usage = 0;

	if (m_type != kStatic /*0*/) {
		pool = D3DPOOL_DEFAULT;

		if (m_type == kRenderTarget /*1*/) {
			usage = D3DUSAGE_RENDERTARGET;

		} else if (m_type == kDepthSurface /*2*/) {
			usage = D3DUSAGE_DEPTHSTENCIL;

		} else if (m_type == kDynamic /*3*/) {
			usage = D3DUSAGE_DYNAMIC;
		}

		// TODO: adjust width/height
	}

	// TODO:
	// FormatInfo *dstFmt = GetDestFormatInfo(m_srcFormat, /*usageFlags=*/1);
	// m_format = dstFmt->format;
	m_format = m_srcFormat;

	if (deviceManager9->getD3DDevice()->CreateTexture(
		m_width,
		m_height,
		levels,
		usage,
		m_format,
		pool,
		&m_pD3DTexture,
		0) < 0
	)
		goto tableflip;

	if (m_pD3DTexture->GetSurfaceLevel(/*Level=*/0, &m_pD3DSurface) < 0)
		goto tableflip;

	if (m_type != kStatic /*0*/ && m_pTextureData)
		UpdateD3DTexture(m_pTextureData, 0, GetByteSize());

	if (m_srcFormat == m_format && m_pTextureData) {
		TextureDataFree(m_pTextureData);
		m_pTextureData = nullptr;
	}

	SetFilter(m_filter);
	return true;

tableflip:
	OnCreateResourceFailed();
	return false;
}

void PCDeviceTexture::internalRelease() {
	// TODO
	PCDeviceBaseTexture::internalRelease();
}

void PCDeviceTexture::Release() {
	delete this;
}

IDirect3DBaseTexture9 *PCDeviceTexture::GetD3DTexture() {
	// TODO: run update callback
	return m_pD3DTexture;
}

void PCDeviceTexture::Upload(void *data) {
	// TODO
}

void PCDeviceTexture::UploadPartial(void *data, uint32_t offset, uint32_t length) {
	uint32_t byteSize = GetByteSize();

	if (m_pD3DTexture) {
		UpdateD3DTexture((char*)data - offset, offset, length);

		// TODO
		if (m_format == m_srcFormat)
			return;
	}

	if (!m_pTextureData)
		m_pTextureData = TextureDataAlloc(byteSize);

	memcpy((char*)m_pTextureData + offset, (char*)data, length);
}

float PCDeviceTexture::GetMipMapLodBias() {
	// TODO
	return 0.0f;
}

void PCDeviceTexture::SetFilter(TextureFilter filter) {
	// TODO
}

uint32_t PCDeviceTexture::GetWidth() {
	return m_width;
}

uint32_t PCDeviceTexture::GetHeight() {
	return m_height;
}


}
