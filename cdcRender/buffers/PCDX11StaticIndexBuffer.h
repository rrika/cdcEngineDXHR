#pragma once
#include "PCDX11IndexBuffer.h"
#include "../PCDX11DeferredUpdate.h"
#include "../PCDX11StaticPool.h"

struct ID3D11Buffer;
namespace cdc {

class PCDX11RenderDevice;

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
	PCDX11StaticIndexBuffer(PCDX11StaticPool *pool, uint32_t count, void *data);

	uint32_t getStartIndex() override { return m_alloc.m_offset / 2; }
	ID3D11Buffer *getD3DBuffer() override { return m_alloc.m_pBuffer; }
	uint32_t getCount() override { return m_numIndices; }
	void method_10() override { /*empty*/ }

	void deferredUpdate(PCDX11RenderDevice*) override;
};

}
