#pragma once
#include <cstdint>

class PPVariable {
public:
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t dword20;
	uint32_t dword24;
	uint32_t dword28;
	uint32_t dword2C;

	virtual ~PPVariable() = default;
};
