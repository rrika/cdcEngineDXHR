#include <cstdio>
#include "cdcSys/Assert.h"
#include "PCDeviceTexture.h"
#include "PCTexture.h"

namespace cdc {

void PCTexture::resFree() {
	renderDevice->DeferredRelease(this);
}

void PCTexture::resFill(void *src, uint32_t size, uint32_t offset) {
	printf("PCTexture::resFill %p %x+%x\n", this, offset, size);

	if (!m_pResourceData)
		m_pResourceData = new PCTextureData;

	// read header
	printf("  read header\n");

	if (offset < sizeof(PCTextureData) /*28*/) {
		m_flags = 0;
		m_class = 0;
		m_nSkipBytes = 0;

		if (m_deviceTexture) {
			// TODO

			m_deviceTexture = nullptr;
		}

		uint32_t remainingHeaderSize = sizeof(PCTextureData) - offset;
		if (remainingHeaderSize > size)
			remainingHeaderSize = size;

		printf("  header read src=%p, %x+%x\n", src, offset, remainingHeaderSize);
		memcpy(((char*)m_pResourceData) + offset, src, remainingHeaderSize);
	}

	printf("  read post-header\n");
	if (offset + size >= sizeof(PCTextureData) /*28*/) {

		// create device texture

		if (!m_deviceTexture) {
			if (m_pResourceData->magicNumber != PCTextureData::kMagicNumber /*PCD9*/)
				FatalError("Corrupt texture data passed into PCTexture constructor!");

			m_flags = m_pResourceData->flags;
			m_class = m_pResourceData->m_class;

			if (m_pResourceData->flags & PCTextureData::kCubeMap /*0x8000*/) {
				// TODO

			} else if (m_pResourceData->flags & PCTextureData::kVolumeMap /*0x4000*/) {
				// TODO

			} else {
				m_deviceTexture = new PCDeviceTexture(
					m_pResourceData->width,
					m_pResourceData->height,
					m_pResourceData->format,
					m_pResourceData->numMipMaps + 1,
					PCDeviceBaseTexture::kStatic,
					kTextureFilterAnisotropic16X
				);
			}
		}

		// read pixels

		char *uploadSrc = (char*)src;
		uint32_t uploadOffset;
		uint32_t uploadSize = size;

		// skip nSkipBytes

		if (offset >= sizeof(PCTextureData) + m_nSkipBytes) {
			uploadOffset = offset - sizeof(PCTextureData) + m_nSkipBytes;

		} else {
			uint32_t remainingSkip = sizeof(PCTextureData) + m_nSkipBytes - offset;
			uploadSrc += remainingSkip;
			if (size > remainingSkip) {
				// skip to first data byte
				uploadSize -= remainingSkip;
				uploadOffset = 0;

			} else {
				// everything is skipped
				uploadSize = 0;
				uploadOffset = 0;
			}
		}

		// skip resource name
		
		if (m_pResourceData->flags & PCTextureData::kResourceName /*0x2000*/) {
			if (uploadOffset >= PCTextureData::kResourceNameSize)
				uploadOffset -= PCTextureData::kResourceNameSize;

			else {
				uint32_t remainingSkip = PCTextureData::kResourceNameSize - uploadOffset;
				if (remainingSkip >= uploadSize)
					remainingSkip = uploadSize;
				uploadSrc += remainingSkip;
				uploadSize -= remainingSkip;
				uploadOffset = 0;
			}
		}

		// upload

		printf("  of %x+%x forward %x+%x to %x+%x\n", offset, size, (uploadSrc-(char*)src), uploadSize, uploadOffset, uploadSize);

		if (uploadSize)
			if (m_deviceTexture) // HACK
				m_deviceTexture->UploadPartial(uploadSrc, uploadOffset, uploadSize);
	}
}

char *PCTexture::resGetBuffer() {
	return (char*)m_pResourceData;
}

void PCTexture::resConstruct() {
	// construction already happened in resFill
	delete m_pResourceData;
	m_pResourceData = 0;
}

void PCTexture::resMethod10() {
	// empty
}


uint32_t PCTexture::getWidth() {
	if (m_deviceTexture)
		return m_deviceTexture->GetWidth();
	return 0;
}

uint32_t PCTexture::getHeight() {
	if (m_deviceTexture)
		return m_deviceTexture->GetHeight();
	return 0;
}

}