#pragma once
#include <cstdint>

namespace dtp {

struct Controls3d { // line 81
	float float0;
	float float4;
	float float8;
	float floatC;
	float float10;
	float float14;
	uint32_t dword18;
	uint16_t playbackType; // 1C
	float position[4]; // 20
};

}
