#pragma once
#include "PCDX11ConstantBuffer.h"

namespace cdc {

class PCDX11DynamicConstantBuffer : public PCDX11ConstantBuffer {
	// uint32_t dword14;
	// uint32_t dword18;
	PCDX11ConstantBufferPool *constantBufferPool;
public:
	PCDX11DynamicConstantBuffer(PCDX11ConstantBufferPool *pool, uint32_t rows)
		: PCDX11ConstantBuffer(rows), constantBufferPool(pool)
	{}
};

}
