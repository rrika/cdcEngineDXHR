#include <d3d11.h>
#include "../PCDX11RenderDevice.h"
#include "PCDX11StaticIndexBuffer.h"

struct ID3D11Buffer;

namespace cdc {

PCDX11StaticIndexBuffer::PCDX11StaticIndexBuffer(PCDX11StaticPool *pool, uint32_t count, void *data) :
	m_numIndices(count),
	m_pData(data),
	m_pMemPool(pool),
	m_isOwnData(data != nullptr)
{
	m_alloc = pool->Alloc(2, count);
	if (data)
		deferredUpdate(pool->m_pRenderDevice); // HACK
}

void PCDX11StaticIndexBuffer::deferredUpdate(PCDX11RenderDevice *renderDevice) {
	D3D11_BOX box = {
		m_alloc.m_offset,                  0, 0,
		m_alloc.m_offset + 2*m_numIndices, 1, 1,
	};
	renderDevice->deviceContext->UpdateSubresource(m_alloc.m_pBuffer, 0, &box, m_pData, 0, 0);
}

}
