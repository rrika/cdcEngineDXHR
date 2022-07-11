#pragma once
#include <cstdint>

class PPPass {
public:
	uint32_t dword4;
	PPTexture *textures; // 8
	uint32_t numTextures; // C
	PPVariable *variables; // 10
	uint32_t numVariables; // 14
	uint8_t byte18;

	virtual ~PPPass() = default;
};
