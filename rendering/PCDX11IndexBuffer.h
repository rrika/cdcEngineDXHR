#pragma once
#include <cstdint>
#include <d3d11_1.h>
#include "CommonIndexBuffer.h"

namespace cdc {

class PCDX11IndexBuffer : public CommonIndexBuffer {
public:
	virtual uint32_t getStartIndex() = 0;
	virtual ID3D11Buffer *getD3DBuffer() = 0;
	virtual uint32_t getCount() = 0;
	virtual void method_10() = 0;
};

class HackIndexBuffer : public PCDX11IndexBuffer {
	ID3D11Buffer *buffer;
public:
	HackIndexBuffer(ID3D11Buffer *buffer) : buffer(buffer) {}

	uint32_t getStartIndex() override { return 0; };
	ID3D11Buffer *getD3DBuffer() override { return buffer; };
	uint32_t getCount() override { return 0; };
	void method_10() override {};
};

}
