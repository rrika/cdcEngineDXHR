#pragma once
#include "PCDX11VertexBuffer.h"
#include "../PCDX11DeviceManager.h"
#include <d3d11.h>

namespace cdc {

class PCDX11SimpleDynamicVertexBuffer :
	public PCDX11VertexBuffer
{
public:
	PCDX11SimpleDynamicVertexBuffer(uint32_t stride, uint32_t numVertices) :
		stride(stride),
		numVertices(numVertices)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth      = stride * numVertices;
		vertexBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		deviceManager->getD3DDevice()->CreateBuffer(&vertexBufferDesc, nullptr, &buffer);
	}

	uint32_t stride; // 4
	uint32_t numVertices; // 8
	ID3D11Buffer *buffer; // C

	uint32_t getBaseVertex() override { return 0; }
	ID3D11Buffer *getD3DBuffer() override { return buffer; }
	uint32_t method0C() override { return numVertices; }
	uint32_t getStride() override { return stride; }
	void method14() override {}

	void *map() {
		D3D11_MAPPED_SUBRESOURCE subresourceData;
		deviceManager->getD3DDeviceContext()->Map(
			buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresourceData);
		return subresourceData.pData;
	}

	void unmap() {
		deviceManager->getD3DDeviceContext()->Unmap(buffer, 0);
	}
};

}
