#pragma once
#include "PCDX11InternalResource.h"
#include "PCDX11IndexBuffer.h"
#include "PCDX11PixelShader.h"

namespace cdc {

class PCDX11StateManager : public PCDX11InternalResource {
	ID3D11DeviceContext *m_deviceContext; // 10
	int m_topology; // B8
	ID3D11Buffer *m_indexBufferD3D; // 188
	PCDX11PixelShader *m_pixelShader; // 198
public:
	PCDX11StateManager();
	PCDX11StateManager(ID3D11DeviceContext *deviceContext) :
		m_deviceContext(deviceContext),
		m_indexBufferD3D(nullptr),
		m_pixelShader(nullptr)
	{}

	void setIndexBuffer(PCDX11IndexBuffer *indexBuffer);
	void setPixelShader(PCDX11PixelShader *pixelShader);
	void setPrimitiveTopology(int topology);

	void internalResource04() override;
	void internalResource08() override;
};

}
