#pragma once
#include <cstdint>
#include <d3d9.h>
#include "../../buffers/CommonIndexBuffer.h"

namespace cdc {

class PCIndexBuffer : public CommonIndexBuffer {
public:
	virtual uint32_t getStartIndex() = 0;
	virtual IDirect3DIndexBuffer9 *getD3DBuffer() = 0;
	virtual uint32_t getCount() = 0;
};

class HackIndexBuffer9 : public PCIndexBuffer {
	IDirect3DIndexBuffer9 *buffer;
public:
	HackIndexBuffer9(IDirect3DIndexBuffer9 *buffer) : buffer(buffer) {}

	uint32_t getStartIndex() override { return 0; };
	IDirect3DIndexBuffer9 *getD3DBuffer() override { return buffer; };
	uint32_t getCount() override { return 0; };
};

}
