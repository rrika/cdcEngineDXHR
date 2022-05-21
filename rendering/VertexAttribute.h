#pragma once
#include <cstdint>

class D3D11_INPUT_ELEMENT_DESC;

namespace cdc {

struct VertexAttributeA {
	uint32_t attribKind;
	uint16_t offset;
	uint8_t format;
};

struct VertexAttributeLayoutA {
	uint32_t hash0;
	uint32_t hash4;
	uint16_t numAttr;
	uint8_t vertStrideA;
	uint8_t vertStrideB;
	uint32_t dwordC;
	VertexAttributeA attrib[];
};

void decodeVertexAttribA(
	D3D11_INPUT_ELEMENT_DESC *dst,
	VertexAttributeA *src,
	uint32_t count,
	bool wineWorkaround);

}
