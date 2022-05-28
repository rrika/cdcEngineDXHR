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

struct VertexAttributeB {
	uint32_t attribKind;
	uint32_t dword04;
	uint16_t order;
	uint16_t field_A;
	float vx;
	float vy;
	float vz;
	float vw;
};

struct VertexAttributeLayoutB {
	uint32_t hash0;
	uint32_t hash4;
	uint32_t numAttribs;
	uint32_t dwordC;
	VertexAttributeB attr[];
};

void decodeVertexAttribA(
	D3D11_INPUT_ELEMENT_DESC *dst,
	VertexAttributeA *src,
	uint32_t count,
	bool wineWorkaround);

}
