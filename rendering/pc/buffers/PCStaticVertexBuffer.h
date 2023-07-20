#pragma once
#include <cstdint>
#include "PCVertexBuffer.h"

struct IDirect3DVertexBuffer9;

namespace cdc {

class PCStaticVertexBuffer :
	public PCVertexBuffer
	// public PCInternalResource
{
	// offset // 10
	// size // 14
	IDirect3DVertexBuffer9 *buffer = nullptr; // 18
	// block // 1C
	uint32_t stride = 0; // 20
	// numVertices // 24
	// baseVertex // 28
	// vertices // 2C
	// owned // 30
	void *pool = nullptr; // 34
public:
	PCStaticVertexBuffer(void *pool) : pool(pool) {}
	void Create(void *data, uint32_t numVertices, uint32_t stride);

	IDirect3DVertexBuffer9 *GetD3DVertexBuffer() override { return buffer; }
	uint16_t GetStride() override { return stride; }
};

}
