#pragma once
#include <d3d11.h>
#include "PCDX11IndexBuffer.h"
#include "../PCDX11DeviceManager.h"
#include "../PCDX11DeferredUpdate.h"
#include "../PCDX11StaticPool.h"

namespace cdc {

class PCDX11StaticIndexBuffer :
	public PCDX11IndexBuffer,
	public PCDX11DeferredUpdate
{
	uint32_t m_numIndices; // C
	void *m_pData; // 10
	PCDX11StaticPool *m_pMemPool; // 14
	PCDX11StaticPool::Allocation m_alloc; // 18
	bool m_isOwnData; // 28

public:
	PCDX11StaticIndexBuffer(PCDX11StaticPool *pool, uint32_t count, void *data) :
		m_numIndices(count),
		m_pData(data),
		m_pMemPool(pool),
		m_isOwnData(data != nullptr)
	{
		// HACK: pool is ignored, acts like PCDX11SimpleStaticIndexBuffer instead

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = 2 * count;
		vertexBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA subresourceData = { data };
		deviceManager->getD3DDevice()->CreateBuffer(&vertexBufferDesc, &subresourceData, &m_alloc.m_pBuffer);
		m_alloc.m_offset = 0;
		m_alloc.m_size = 2 * count;
	}

	uint32_t getStartIndex() override { return m_alloc.m_offset; }
	ID3D11Buffer *getD3DBuffer() override { return m_alloc.m_pBuffer; }
	uint32_t getCount() override { return m_numIndices; }
	void method_10() override { /*empty*/ }

	void deferredUpdate(PCDX11RenderDevice*) override { /*TODO*/ }
};

}
