#pragma once
#include <d3d11.h>
#include "PCDX11VertexBuffer.h"
#include "../PCDX11DeferredUpdate.h"
#include "../PCDX11DeviceManager.h"
#include "../PCDX11StaticPool.h"

namespace cdc {

class PCDX11StaticVertexBuffer :
	public PCDX11VertexBuffer,
	public PCDX11DeferredUpdate
{
	uint32_t stride; // C
	uint32_t count; // 10
	uint32_t baseVertex; // 14
	PCDX11StaticPool::Allocation allocation;
	PCDX11StaticPool *staticPool;
	void *data;

public:
	PCDX11StaticVertexBuffer(PCDX11StaticPool *pool, uint32_t stride, uint32_t count, void *data) :
		stride(stride),
		count(count),
		baseVertex(0),
		staticPool(pool),
		data(data)
	{
		// HACK: pool is ignored, acts like PCDX11SimpleStaticVertexBuffer instead

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = stride * count;
		vertexBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA subresourceData = { data };
		deviceManager->getD3DDevice()->CreateBuffer(&vertexBufferDesc, &subresourceData, &allocation.buffer);
		allocation.totalSize = stride * count;
	}

	uint32_t getBaseVertex() override { return baseVertex; }
	ID3D11Buffer *getD3DBuffer() override { return allocation.buffer; }
	uint32_t method0C() override { return count; }
	uint32_t getStride() override { return stride; }
	void method14() override {}

	void deferredUpdate(PCDX11RenderDevice*) override { /*TODO*/ }
};

}
