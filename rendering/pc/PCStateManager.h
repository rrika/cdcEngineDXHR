#pragma once
#include "PCInternalResource.h"
#include "buffers/PCIndexBuffer.h"

namespace cdc {

class PCPixelShader;
class PCVertexBuffer;
class PCVertexShader;

class PCStateManager : public PCInternalResource {
	IDirect3DDevice9 *m_device; // C
	IDirect3DIndexBuffer9 *m_indexBufferD3D; // 14
	PCVertexBuffer *m_vertexBuffer; // 18
	PCPixelShader *m_pixelShader; // 20
	PCVertexShader *m_vertexShader; // 24

public:
	PCStateManager();
	PCStateManager(IDirect3DDevice9 *device) :
		m_device(device),
		m_indexBufferD3D(nullptr)
	{}

	void setIndexBuffer(PCIndexBuffer *indexBuffer);
	void setVertexBuffer(PCVertexBuffer *vertexBuffer);
	void setPixelShader(PCPixelShader *pixelShader);
	void setVertexShader(PCVertexShader *vertexShader);

	bool internalCreate() override;
	void internalRelease() override;
};

}
