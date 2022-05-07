#pragma once
#include <cstdint>

class ID3D11Buffer;

namespace cdc {

class PCDX11ConstantBuffer {
public:
	ID3D11Buffer *buffer;
	void *data;
	uint32_t sizeInUnitsOf16;
	uint32_t size;
public:
	PCDX11ConstantBuffer(uint32_t rows)
		: buffer(nullptr), data(nullptr), sizeInUnitsOf16(rows), size(rows*16)
	{}
	virtual ~PCDX11ConstantBuffer() = default;
	virtual ID3D11Buffer *getBuffer() { return buffer; }
};

}
