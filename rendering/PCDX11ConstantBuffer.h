#pragma once
#include <cstdint>
#include <cstring>

class ID3D11Buffer;
struct float4x4;

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

	void assignRow(uint32_t index, void *rowData, uint32_t numRows) {
		memcpy(((char*)data) + (index * 16), rowData, numRows * 16);
	}

	void assignMatrix(uint32_t index, float4x4& matrix) {
		assignRow(index, (void*)&matrix, 4);
	}
};

}
