#pragma once
#include <cstdint>
#include "rendering/buffers/CommonVertexBuffer.h"

struct IDirect3DVertexBuffer9;

namespace cdc {

class PCVertexBuffer : public CommonVertexBuffer {
public:
	virtual uint32_t GetBaseVertexIndex() = 0;
	virtual IDirect3DVertexBuffer9 *GetD3DVertexBuffer() = 0;
	// virtual uint32_t GetNumVertices() = 0;
	virtual uint16_t GetStride() = 0;
};

class HackVertexBuffer9 : public PCVertexBuffer {
	IDirect3DVertexBuffer9 *buffer;
	uint32_t stride;
public:
	HackVertexBuffer9(IDirect3DVertexBuffer9 *buffer, uint32_t stride)
		: buffer(buffer), stride(stride) {}
	uint32_t GetBaseVertexIndex() override { return 0; }
	IDirect3DVertexBuffer9 *GetD3DVertexBuffer() override { return buffer; }
	uint16_t GetStride() override { return stride; }
};


}
