#pragma once
#include "AnimFragment.h"

class Instance;

namespace cdc {

class AnimPose {
public:
	// uint32_t dword0;
	// uint32_t dword4;
	AnimBuffer *buffer=nullptr; // 8
	// uint32_t dwordC;
	bool clearedByFragment; // 10

	void AllocSegs(uint32_t numSegments, Instance *instance, uint32_t);
	void ClearSegValues(float scale);
};

}