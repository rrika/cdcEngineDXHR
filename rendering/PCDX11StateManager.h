#pragma once
#include "PCDX11InternalResource.h"
#include "PCDX11IndexBuffer.h"

namespace cdc {

class PCDX11StateManager : public PCDX11InternalResource {
	ID3D11DeviceContext *m_deviceContext;
	ID3D11Buffer *m_indexBufferD3D;
public:
	PCDX11StateManager();
	PCDX11StateManager(ID3D11DeviceContext *deviceContext) :
		m_deviceContext(deviceContext),
		m_indexBufferD3D(nullptr)
	{}

	void setIndexBuffer(PCDX11IndexBuffer *indexBuffer);

	void internalResource04() override;
	void internalResource08() override;
};

}
