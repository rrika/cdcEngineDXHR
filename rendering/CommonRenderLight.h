#pragma once
#include "cdcMath/Math.h" // for Matrix
#include "IRenderLight.h"
#include <cstdint>

namespace cdc {

class CommonRenderLight : public IRenderLight {
public:
	uint32_t dword4;
	uint8_t f8[8];
	Matrix mat10;
	float float50[3];
	uint8_t f5C[28];
	float float74;
	float float78;
	float float7C;
	uint8_t byte84;
	uint8_t f85[3];
	float float88;
	float float8C;
	float float90;
	uint32_t dword94;
	float float98;
	uint8_t f9C[4];
	Matrix matA0;
};

}
