#pragma once
#include <cstdint>

class D3D11_INPUT_ELEMENT_DESC;

namespace cdc {

struct VertexAttributeA { // VertexElem

	static const uint32_t kPosition = 0xD2F7D823;
	static const uint32_t kNormal = 0x36F5E414;
	static const uint32_t kTessellationNormal = 0; // TODO
	static const uint32_t kTangent = 0XF1ED11C3;
	static const uint32_t kBinormal = 0x64A86F01;
	static const uint32_t kPackedNTB = 0; // TODO
	static const uint32_t kSkinWeights = 0; // TODO
	static const uint32_t kSkinIndices = 0; // TODO
	static const uint32_t kColor1 = 0X7E7DD623;
	static const uint32_t kColor2 = 0; // TODO
	static const uint32_t kTexcoord1 = 0x8317902A;
	static const uint32_t kTexcoord2 = 0x8E54B6F3;
	static const uint32_t kTexcoord3 = 0; // TODO
	static const uint32_t kTexcoord4 = 0; // TODO

	static const uint32_t kTexcoord0 = 0xFFFF0000; // HACK

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
	uint32_t attribKindA; // hash
	uint32_t nextAttribIndex;
	uint16_t attribKindB; // enum
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

uint16_t getLayoutAIndexFromHash(
	VertexAttributeLayoutA *layoutA,
	uint32_t hash);

void decodeVertexAttribA(
	D3D11_INPUT_ELEMENT_DESC *dst,
	VertexAttributeA *src,
	uint32_t count,
	bool wineWorkaround);

void semanticFromEnum(
	D3D11_INPUT_ELEMENT_DESC *elem,
	int e);

}
