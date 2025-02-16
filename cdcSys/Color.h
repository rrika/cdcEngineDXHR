#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc {

using Color = Vector;
using ColorArg = Color const&;

extern const Color ColorRED;
extern const Color ColorGREEN;
extern const Color ColorBLUE;
extern const Color ColorCYAN;
extern const Color ColorMAGENTA;
extern const Color ColorYELLOW;
extern const Color ColorPURPLE;
extern const Color ColorORANGE;
extern const Color ColorBROWN;
extern const Color ColorDARKMAGENTA;
extern const Color ColorDARKRED;
extern const Color ColorDARKBLUE;
extern const Color ColorDARKGREEN;
extern const Color ColorDARKYELLOW;
extern const Color ColorLIGHTMAGENTA;
extern const Color ColorLIGHTRED;
extern const Color ColorLIGHTBLUE;
extern const Color ColorLIGHTGREEN;
extern const Color ColorLIGHTYELLOW;
extern const Color ColorLIGHTGRAY;
extern const Color ColorMEDGRAY;
extern const Color ColorDARKGRAY;
extern const Color Color50Percent;
extern const Color ColorFADEDDARKGREEN;

}

inline void SetARGB(float *color, uint32_t argb) { // method of cdc::Color
	color[0] = (argb >> 16 & 255) / 255.f;
	color[1] = (argb >>  8 & 255) / 255.f;
	color[2] = (argb >>  0 & 255) / 255.f;
	color[3] = (argb >> 24 & 255) / 255.f;
}
