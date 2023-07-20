#pragma once
#include <cstdint>
#include "PCIndexBuffer.h"

struct IDirect3DIndexBuffer9;

namespace cdc {

class PCStaticIndexBuffer : public PCIndexBuffer {
	void *pool = nullptr; // 10
	// offset // 14
	// size // 18
	IDirect3DIndexBuffer9 *buffer = nullptr; // 1C
	// block // 20
	uint32_t numIndices = 0; // 24
	// startIndex // 28
	// uint16_t *indices = nullptr; // 2C

public:
	PCStaticIndexBuffer(void *pool) : pool(pool) {}
	void Create(uint16_t *indices, uint32_t numIndices);

	uint32_t getStartIndex() override { return 0; };
	IDirect3DIndexBuffer9 *getD3DBuffer() override { return buffer; };
	uint32_t getCount() override { return numIndices; };
};

}
