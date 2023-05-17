#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

struct PackedBytesAndScale {
	uint32_t bytes;
	float scale;

	cdc::Vector unpack();
};
