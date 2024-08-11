#pragma once
#include "PCDX11VertexBuffer.h"
#include "../PCDX11Pool.h"

namespace cdc {

class PCDX11DynamicVertexBuffer : public PCDX11VertexBuffer {
	uint16_t m_stride; // 4
	uint32_t m_numVertices; // 8
	PCDX11Pool::Allocation m_alloc; // C
	PCDX11Pool *m_pMemPool; // 24

public:
	PCDX11DynamicVertexBuffer(PCDX11Pool *pool, uint32_t stride, uint32_t numVertices);

	uint32_t getBaseVertex() override;
	ID3D11Buffer *getD3DBuffer() override;
	uint32_t method0C() override;
	uint32_t getStride() override;
	void method14() override;

	unsigned char *Lock();
};

}
