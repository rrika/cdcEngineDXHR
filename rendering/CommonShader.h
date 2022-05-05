#pragma once
#include <cstdint>

namespace cdc {

struct DXBCWrapper { // guessed name
	uint32_t baadf00d;
	uint32_t size;
	uint32_t dword8;
	uint32_t dwordC;
	char dxbc[];
};

struct ShaderSub { // guessed name
	DXBCWrapper *dxbcWrapper;
	void *shaderBinary;
	uint16_t word8;
	uint16_t shaderBinarySize;
};

}
