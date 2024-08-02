#include <d3d11.h>
#include "../PCDX11RenderDevice.h"
#include "PCDX11StaticVertexBuffer.h"

struct ID3D11Buffer;

namespace cdc {

PCDX11StaticVertexBuffer::PCDX11StaticVertexBuffer(PCDX11StaticPool *pool, uint32_t stride, uint32_t count, void *data) :
	stride(stride),
	count(count),
	m_pMemPool(pool),
	m_pData(data)
{
	m_alloc = pool->Alloc(stride, count);
	baseVertex = m_alloc.m_offset / stride;
	if (data)
		deferredUpdate(pool->m_pRenderDevice); // HACK
}

void PCDX11StaticVertexBuffer::deferredUpdate(PCDX11RenderDevice *renderDevice) {
	// TODO: confirm this
	D3D11_BOX box = {
		m_alloc.m_offset,                0, 0,
		m_alloc.m_offset + stride*count, 1, 1,
	};
	renderDevice->deviceContext->UpdateSubresource(m_alloc.m_pBuffer, 0, &box, m_pData, 0, 0);
}

}
