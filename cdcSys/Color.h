#pragma once
#include "cdcMath/Math.h"

inline void SetARGB(float *color, uint32_t argb) { // method of cdc::Color
	color[0] = (argb >> 16 & 255) / 255.f;
	color[1] = (argb >>  8 & 255) / 255.f;
	color[2] = (argb >>  0 & 255) / 255.f;
	color[3] = (argb >> 24 & 255) / 255.f;
}
