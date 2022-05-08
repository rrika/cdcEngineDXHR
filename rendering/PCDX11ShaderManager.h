#pragma once
#include <cstdint>
#include "PCDX11VertexShader.h"
#include "PCDX11PixelShader.h"

namespace cdc {

class PCDX11ShaderManager {
	// uint32_t dword0;
	// uint32_t dword4;
	// uint32_t dword8;
	// uint32_t dwordC;
	// uint32_t dword10;
	// uint32_t dword14;
	// uint32_t dword18;
	// uint32_t dword1C;
	// uint32_t dword20;
	// uint32_t dword24;
	// uint32_t dword28;
	// uint32_t dword2C;
	// uint32_t dword30;
	// uint32_t dword34;
	// uint32_t dword38;
	// uint32_t dword3C;
	// uint8_t gap40[1028];
	// uint8_t gap444[1028];
	// uint8_t gap848[1028];
	// uint8_t gapC4C[1028];
	// uint8_t gap1050[1028];
public:
	PCDX11PixelShader *createPixelShader(char *blob, bool takeCopy, bool isWrapped) {
		// uint32_t a = ++dword4;
		// uint32_t b = dwordC;
		// dwordC = a > b ? a : b;
		auto *shader = PCDX11PixelShader::create(blob, takeCopy, /*gap40,*/ &isWrapped, isWrapped);
		// if (isWrapped)
		// 	dword8++;
		return shader;
	}
	PCDX11VertexShader *createVertexShader(char *blob, bool takeCopy, bool isWrapped) {
		// uint32_t a = ++dword10;
		// uint32_t b = dword18;
		// dword18 = a > b ? a : b;
		auto *shader = PCDX11VertexShader::create(blob, takeCopy, /*gap444,*/ &isWrapped, isWrapped);
		// if (isWrapped)
		// 	dword14++;
		return shader;
	}
};

}
