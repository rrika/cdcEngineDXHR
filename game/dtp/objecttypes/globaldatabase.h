#pragma once
#include <cstdint>

struct ScaleformString {
	char *string;
	uint32_t stringId;
};

struct GlobalDatabase {
	char pad0[0x8c];
	ScaleformString *scaleformStrings; // 8c
	uint32_t numScaleformStrings; // 90
	char pad94[0x13c];
	const char *newGameMap; // 1d0
};
