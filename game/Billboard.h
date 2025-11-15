#pragma once
#include <cstdint>

namespace cdc { class IMaterial; class Vector; struct Matrix; }

struct Billboard0 {
	float sizeX; // 0, in pixels on a reference 1280x720 screen
	float sizeY; // 4, in pixels on a reference 1280x720 screen
	float oppositeness; // 8
};

struct Billboard1 {
	float x; // 0
	float y; // 4
	bool flag; // 8
};

struct Billboard2 { // TODO: recheck layout
	float x0; // 0
	float y0; // 4
	float z0; // 8

	float x1; // C
	float y1; // 10

	uint32_t submode; // 14
	float x2; // 18
	float y2; // 1C
	float z2; // 20
	bool flag2; // 24
};

static_assert(sizeof(Billboard2) == 0x28);

struct Billboard {
	cdc::IMaterial *material; // 0
	float u0, v0; // 4
	float u1, v1; // C
	uint32_t color; // 14
	uint32_t mode; // 18
	union { // 1C
		Billboard0 mode0;
		Billboard1 mode1;
		Billboard2 mode2;
	};
	bool depthFuncLE; // 44 (0=always, 1=less-or-equal)
};

static_assert(sizeof(Billboard) == 0x48);

void DrawBillboards(
	cdc::Matrix& matrix,
	uint32_t numBillboards,
	Billboard *billboards,
	cdc::Vector tint,
	cdc::Vector& param0,
	float zoffset);
