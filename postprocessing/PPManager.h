#pragma once
#include <cstdint>
#include <vector>

struct PPUnknown1 {
	// TODO (sizeof=88)
};

struct PPUnknown2 {
	// TODO (sizeof=48)
};

class PPManager {
public:
	std::vector<PPUnknown1*> unknowns1; // 4
	std::vector<PPTexture*> textures; // 10
	std::vector<PPVariable*> variables; // 1C
	uint32_t dword28;
	uint8_t char2C[128];
	uint8_t charAC[128];
	uint8_t char12C[128];
	uint32_t dword1AC;
	uint32_t dword1B0;
	std::vector<PPUnknown2*> unknown2; // 1B4

	virtual ~PPManager() = default;
};
