#pragma once
#include <cstdint>

namespace cdc {

struct PCShaderBinary {
	void *header = nullptr; // 0
	void *shaderBinary = nullptr; // 4
	bool hasOwnership; // 8

	PCShaderBinary(char *origBlob, bool takeCopy, bool isWrapped) {
		hasOwnership = takeCopy;
		char *blob = origBlob;
		if (takeCopy) {
			uint32_t size = *(uint16_t*)(blob + 4);
			blob = new char[size];
			memcpy(blob, origBlob, size);
		}
		if (isWrapped) {
			header = blob;
			shaderBinary = blob + 16;
		} else {
			header = nullptr; // TODO
			shaderBinary = blob;
		}
	}
};

}
