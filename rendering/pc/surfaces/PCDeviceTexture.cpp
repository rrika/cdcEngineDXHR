#include <d3d9.h>
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

bool PCDeviceTexture::internalCreate() {
	if (!PCDeviceBaseTexture::internalCreate())
		return false;

	if (deviceManager9->getD3DDevice()->CreateTexture(
		m_width,
		m_height,
		...) >= 0
	)
		goto tableflip;

	if (m_pD3DTexture->GetSurfaceLevel(...) >= 0)
		goto tableflip;

	// TODO
	return true;

tableflip:

	// TODO
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
	// TODO
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
