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

struct PCDX11ShaderBinary { // guessed name
	DXBCWrapper *dxbcWrapper = nullptr;
	void *shaderBinary = nullptr;
	bool hasOwnership; // 8
	bool wineWorkaround = false; // 9
	uint16_t shaderBinarySize; // C
public:
	PCDX11ShaderBinary() { // hack
		hasOwnership = false;
	}

	PCDX11ShaderBinary(void *shaderBinary, uint16_t shaderBinarySize) :
		dxbcWrapper(nullptr),
		shaderBinary(shaderBinary),
		hasOwnership(false),
		shaderBinarySize(shaderBinarySize)
	{} // hack

	PCDX11ShaderBinary(char *blob, bool takeCopy, bool isWrapped) {
		hasOwnership = takeCopy;
		init(blob, takeCopy, isWrapped);
	}

	~PCDX11ShaderBinary() {
		if (hasOwnership) {
			if (dxbcWrapper) // TODO
				delete[] ((char*)shaderBinary-16);
			else
				delete[] (char*)shaderBinary;
		}
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
			// shaderBinarySize = *(uint32_t*)(blob + 4);

			// HACK: game says to do this above but I don't understand why
			//       and offset 24 is conveniently where DXBC puts the size
			shaderBinarySize = *(uint32_t*)(blob + 24);
		}
	}
};

}
