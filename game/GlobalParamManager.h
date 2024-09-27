#pragma once
#include "game/PackedVector.h"

struct GlobalParamInit {
	PackedBytesAndScale packedVectors[8]; // 0
	cdc::Vector plainVectors[8]; // 40
	uint32_t selector[11];
};

static_assert(sizeof(GlobalParamInit) == 0xec);

void ApplyGlobalParams(cdc::Vector *globalParams, GlobalParamInit const *init);
