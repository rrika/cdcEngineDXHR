#pragma once
#include <cstdint>

struct GlobalPlayerInfo {
	char pad0[0xE48];
	uint32_t numJensens; // E48
	uint16_t *jensens; // E4C
};
