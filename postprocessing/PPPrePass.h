#pragma once
#include <cstdint>

class PPPrePass {
public:
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	uint8_t byte18;
	uint8_t byte19;

	virtual ~PPPrePass() = default;
};
