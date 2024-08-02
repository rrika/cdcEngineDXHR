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
	PCDX11StaticPool::Allocation m_alloc;
	PCDX11StaticPool *m_pMemPool;
	void *m_pData;

public:
	PCDX11StaticVertexBuffer(PCDX11StaticPool *pool, uint32_t stride, uint32_t count, void *data);

	uint32_t getBaseVertex() override { return baseVertex; }
	ID3D11Buffer *getD3DBuffer() override { return m_alloc.m_pBuffer; }
	uint32_t method0C() override { return count; }
	uint32_t getStride() override { return stride; }
	void method14() override {}

	void deferredUpdate(PCDX11RenderDevice*) override;
};

}
