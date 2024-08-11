#include "PCDX11DynamicIndexBuffer.h"

namespace cdc {

PCDX11DynamicIndexBuffer::PCDX11DynamicIndexBuffer(PCDX11Pool *pool, uint32_t numIndices) :
	m_numIndices(numIndices),
	m_pMemPool(pool)
{
	m_alloc = pool->Alloc(2, numIndices);
}

uint32_t PCDX11DynamicIndexBuffer::getStartIndex() { return m_alloc.m_offset / sizeof(uint16_t); }
ID3D11Buffer *PCDX11DynamicIndexBuffer::getD3DBuffer() { return m_alloc.m_buffer; }
uint32_t PCDX11DynamicIndexBuffer::getCount() { return m_numIndices; }
void PCDX11DynamicIndexBuffer::method_10() {
	m_pMemPool->Update(m_alloc);
}

unsigned char *PCDX11DynamicIndexBuffer::Lock() {
	return m_pMemPool->Lock(m_alloc);
}

}
