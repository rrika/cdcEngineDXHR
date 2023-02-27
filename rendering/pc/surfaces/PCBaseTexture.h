#pragma once
#include "PCDeviceBaseTexture.h"

namespace cdc {

class PCBaseTexture {
protected:
	PCDeviceBaseTexture *m_deviceTexture; // 4
	uint32_t m_flags; // 8
	uint32_t m_class; // C

public:
	PCBaseTexture(PCDeviceBaseTexture *deviceTexture, uint32_t flags, uint32_t texClass) :
		m_deviceTexture(deviceTexture),
		m_flags(flags),
		m_class(texClass)
	{}

	PCDeviceBaseTexture *GetDeviceBaseTexture() { return m_deviceTexture; }
	virtual bool NeedsUpdate() { return false; }
	virtual void Update() {}
};

}
