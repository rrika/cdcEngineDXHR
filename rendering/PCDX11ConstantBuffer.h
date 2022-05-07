#pragma once
#include <cstdint>

class ID3D11Buffer;

namespace cdc {

class PCDX11ConstantBuffer {
	ID3D11Buffer *buffer;
	void *data;
	uint32_t sizeInUnitsOf16;
	uint32_t size;
public:
	virtual ~PCDX11ConstantBuffer() = default;
	virtual ID3D11Buffer *getBuffer() { return buffer; }
};

}
