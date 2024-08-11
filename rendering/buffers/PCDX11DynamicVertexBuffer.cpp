#include "PCDX11DynamicVertexBuffer.h"

namespace cdc {

PCDX11DynamicVertexBuffer::PCDX11DynamicVertexBuffer(PCDX11Pool *pool, uint32_t stride, uint32_t numVertices) :
	m_stride(stride),
	m_numVertices(numVertices),
	m_pMemPool(pool)
{
	m_alloc = pool->Alloc(stride, numVertices);
}

uint32_t PCDX11DynamicVertexBuffer::getBaseVertex() { return m_alloc.m_offset / m_stride; }
ID3D11Buffer *PCDX11DynamicVertexBuffer::getD3DBuffer() { return m_alloc.m_buffer; }
uint32_t PCDX11DynamicVertexBuffer::method0C() { return m_numVertices; }
uint32_t PCDX11DynamicVertexBuffer::getStride() { return m_stride; }
void PCDX11DynamicVertexBuffer::method14() {
	m_pMemPool->Update(m_alloc);
}

unsigned char *PCDX11DynamicVertexBuffer::Lock() {
	return m_pMemPool->Lock(m_alloc);
}

}
