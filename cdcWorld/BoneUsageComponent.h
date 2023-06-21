#pragma once
#include <cstring>
#include "InstanceComponent.h"

namespace cdc {

class BoneUsageComponent : public InstanceComponent{
	// in Tomb Raider, cdc::RenderModels keep around a cdc::ModelData with a uint32_t boneUsageMap[8]
public:
	BoneUsageComponent() {
		memset(mapA, 0, sizeof(mapA));
		memset(mapB, 0, sizeof(mapB));
	}
	// uint32_t dword0;
	// uint32_t dword4;
	// uint32_t dword8;
	// uint32_t dwordC;
	uint32_t mapA[8]; // 10
	uint32_t mapB[8]; // 30
};

}
