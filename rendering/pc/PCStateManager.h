#pragma once
#include "PCInternalResource.h"
#include "buffers/PCIndexBuffer.h"

namespace cdc {

class PCStateManager : public PCInternalResource {
	IDirect3DDevice9 *m_device; // C
	IDirect3DIndexBuffer9 *m_indexBufferD3D; // 14
public:
	PCStateManager();
	PCStateManager(IDirect3DDevice9 *device) :
		m_device(device),
		m_indexBufferD3D(nullptr)
	{}

	void setIndexBuffer(PCIndexBuffer *indexBuffer);

	bool internalCreate() override;
	void internalRelease() override;
};

}
