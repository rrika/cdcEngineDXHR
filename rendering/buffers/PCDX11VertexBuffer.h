#pragma once
#include "CommonVertexBuffer.h"
#include <cstdint>

struct ID3D11Buffer;

namespace cdc {

class PCDX11VertexBuffer : public CommonVertexBuffer {
public:
	virtual uint32_t getBaseVertex() = 0;
	virtual ID3D11Buffer *getD3DBuffer() = 0;
	virtual uint32_t method0C() = 0;
	virtual uint32_t getStride() = 0;
	virtual void method14() = 0;
};

}
