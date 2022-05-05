#pragma once
#include "PCDX11VertexBuffer.h"
#include "PCDX11DeviceManager.h"

namespace cdc {

class PCDX11StaticVertexBuffer :
	public PCDX11VertexBuffer
	// public PCDX11DeferredUpdate
{
public:
	PCDX11StaticVertexBuffer(uint32_t stride, uint32_t count, void *data) :
		stride(stride)
	{
		// hack
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = stride * count;
		vertexBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA subresourceData = { data };
		deviceManager->getD3DDevice()->CreateBuffer(&vertexBufferDesc, &subresourceData, &buffer);
	}

	PCDX11StaticVertexBuffer(ID3D11Buffer* buffer, uint32_t stride) : // hack
		stride(stride),
		buffer(buffer)
	{}

	uint32_t stride;
	uint32_t dword10;
	uint32_t baseVertex;
	ID3D11Buffer *buffer;

	uint32_t getBaseVertex() { return baseVertex; }
	ID3D11Buffer *getD3DBuffer() { return buffer; }
	uint32_t method0C() { return dword10; }
	uint32_t getStride() { return stride; }
	void method14() {}
};

}
