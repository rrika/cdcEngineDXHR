#pragma once
#include <cstdint>
#include <cstring>

namespace cdc {

struct DXBCWrapper { // guessed name
	uint32_t baadf00d;
	uint32_t size;
	uint32_t dword8;
	uint32_t dwordC;
	// wrapped shader follows
};

static_assert(sizeof(DXBCWrapper) == 16);

struct PCDX11ShaderSub { // guessed name
	DXBCWrapper *dxbcWrapper = nullptr;
	void *shaderBinary = nullptr;
	bool hasOwnership; // 8
	uint16_t shaderBinarySize; // C
public:
	PCDX11ShaderSub() { // hack
		hasOwnership = false;
	}

	PCDX11ShaderSub(void *shaderBinary, uint16_t shaderBinarySize) :
		dxbcWrapper(nullptr),
		shaderBinary(shaderBinary),
		hasOwnership(false),
		shaderBinarySize(shaderBinarySize)
	{} // hack

	PCDX11ShaderSub(char *blob, bool takeCopy, bool isWrapped) {
		hasOwnership = takeCopy;
		init(blob, takeCopy, isWrapped);
	}

	void init(char *origBlob, bool takeCopy, bool isWrapped) {
		char *blob = origBlob;
		if (takeCopy) {
			uint32_t size = *(uint16_t*)(blob + 4);
			blob = new char[size];
			memcpy(blob, origBlob, size);
		}
		if (isWrapped) {
			dxbcWrapper = (DXBCWrapper *)blob;
			shaderBinary = blob + 16;
			shaderBinarySize = dxbcWrapper->size - 16;
		} else {
			dxbcWrapper = nullptr; // TODO
			shaderBinary = blob;
			shaderBinarySize = *(uint32_t*)(blob + 4);
		}
	}
};

}
