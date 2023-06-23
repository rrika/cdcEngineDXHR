#pragma once
#include <cstdint>
#include "rendering/buffers/CommonVertexBuffer.h"

struct IDirect3DVertexBuffer9;

namespace cdc {

class PCVertexBuffer : public CommonVertexBuffer {
public:
	// virtual uint32_t GetBaseVertexIndex() = 0;
	virtual IDirect3DVertexBuffer9 *GetD3DVertexBuffer() = 0;
	// virtual uint32_t GetNumVertices() = 0;
	virtual uint16_t GetStride() = 0;
};

}
