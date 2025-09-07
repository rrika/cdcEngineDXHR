#pragma once
#include <cstdint>
#include "game/GlobalParamManager.h"

struct GlobalParamInit;

struct ScaleformString {
	char *string;
	uint32_t stringId;
};

struct GoalDesc {
	const char *name;
	char pad4[0x74];
};

struct GlobalDatabase {
	uint32_t pad0;
	uint32_t pad4;
	uint32_t numGoals; // 8
	GoalDesc *goals; // C
	char pad10[0x7c];
	ScaleformString *scaleformStrings; // 8C
	uint32_t numScaleformStrings; // 90
	char pad94[0x13c];
	const char *newGameMap; // 1D0
	uint32_t pad1d4;
	uint32_t pad1d8;
	uint32_t numBarksEvents; // 1DC
	const char **barkEvents; // 1E0
	char pad1e4[0x2e0];
	GlobalParamInit globalParamInit; // 4C4
};
