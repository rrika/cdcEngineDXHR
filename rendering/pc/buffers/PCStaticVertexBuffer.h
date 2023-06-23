#pragma once
#include <cstdint>
#include "PCVertexBuffer.h"
#include "rendering/buffers/CommonVertexBuffer.h"

struct IDirect3DVertexBuffer9;

namespace cdc {

class PCStaticVertexBuffer : public PCVertexBuffer {
	IDirect3DVertexBuffer9 *buffer; // 18
	uint32_t stride; // 20
public:
	PCStaticVertexBuffer(IDirect3DVertexBuffer9 *buffer, uint32_t stride)
		: buffer(buffer), stride(stride) {}
	IDirect3DVertexBuffer9 *GetD3DVertexBuffer() override { return buffer; }
	uint16_t GetStride() override { return stride; }
};

}
