#pragma once
#include <cstdint>

namespace dtp {

struct AnimEntry {
	uint16_t animID;
	uint16_t word2;
	uint32_t dword4;
	uint32_t dword8;
};

}