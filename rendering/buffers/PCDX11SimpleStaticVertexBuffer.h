#pragma once
#include "PCDX11VertexBuffer.h"
#include "../PCDX11DeviceManager.h"
#include <d3d11.h>

namespace cdc {

class PCDX11SimpleStaticVertexBuffer :
	public PCDX11VertexBuffer
{
public:
	PCDX11SimpleStaticVertexBuffer(uint32_t stride, uint32_t count, void *data) :
		stride(stride),
		count(count)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = stride * count;
		vertexBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA subresourceData = { data };
		deviceManager->getD3DDevice()->CreateBuffer(&vertexBufferDesc, &subresourceData, &buffer);
	}

	~PCDX11SimpleStaticVertexBuffer() {
		if (buffer)
			buffer->Release();
	}

	uint32_t count;
	uint32_t stride;
	ID3D11Buffer *buffer;

	uint32_t getBaseVertex() { return 0; }
	ID3D11Buffer *getD3DBuffer() { return buffer; } // getDwordC
	uint32_t method0C() { return count; } // getDword4
	uint32_t getStride() { return stride; } // getDword8
	void method14() {}
};

}
