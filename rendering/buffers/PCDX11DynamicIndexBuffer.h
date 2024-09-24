#pragma once
#include "PCDX11IndexBuffer.h"
#include "../PCDX11Pool.h"

struct ID3D11Buffer;
namespace cdc {

class PCDX11DynamicIndexBuffer : public PCDX11IndexBuffer {
	uint32_t m_numIndices;
	uint16_t *m_pData;
	PCDX11Pool *m_pMemPool; // C
	PCDX11Pool::Allocation m_alloc; // 10

public:
	PCDX11DynamicIndexBuffer(PCDX11Pool *pool, uint32_t numIndices);

	uint32_t getStartIndex() override;
	ID3D11Buffer *getD3DBuffer() override;
	uint32_t getCount() override;
	void method_10() override;

	unsigned char *Lock();
};

// Unless P1077R0 is accepted, the virtual CommonIndexBuffer dtor prevents this from being trivial
// static_assert(std::is_trivially_destructible<PCDX11DynamicIndexBuffer>(),
// 	"PCDX11DynamicIndexBuffer might be allocated from the transient heap for which dtors don't run");

}
