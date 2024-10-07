#pragma once
#include <cstdint>
#include "PCDX11ComputeShader.h"
#include "PCDX11PixelShader.h"
#include "PCDX11VertexShader.h"

namespace cdc {

struct ShaderManagerSub {
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
};

class PCDX11ShaderManager {
	// uint32_t dword0;
	// ShaderManagerSub sub4;
	// ShaderManagerSub sub10;
	// ShaderManagerSub sub1C;
	// ShaderManagerSub sub28;
	// ShaderManagerSub sub34;
	// uint8_t gap40[1028];
	// uint8_t gap444[1028];
	// uint8_t gap848[1028];
	// uint8_t gapC4C[1028];
	// uint8_t gap1050[1028];
public:
	PCDX11PixelShader *createPixelShader(char *blob, bool takeCopy, bool isWrapped) {
		// uint32_t a = ++sub4.dword0;
		// uint32_t b = sub4.dword8;
		// sub4.dword8 = a > b ? a : b;
		auto *shader = PCDX11PixelShader::create(blob, takeCopy, /*gap40,*/ &isWrapped, isWrapped);
		// if (isWrapped)
		// 	sub4.dword4++;
		return shader;
	}
	PCDX11VertexShader *createVertexShader(char *blob, bool takeCopy, bool isWrapped) {
		// uint32_t a = ++sub10.dword4;
		// uint32_t b = sub10.dword8;
		// sub10.dword8 = a > b ? a : b;
		auto *shader = PCDX11VertexShader::create(blob, takeCopy, /*gap444,*/ &isWrapped, isWrapped);
		// if (isWrapped)
		// 	sub10.dword0++;
		return shader;
	}
	PCDX11ComputeShader *createComputeShader(char *blob, bool takeCopy, bool isWrapped) {
		// uint32_t a = ++sub34.dword4;
		// uint32_t b = sub34.dword8;
		// sub34.dword8 = a > b ? a : b;
		auto *shader = PCDX11ComputeShader::create(blob, takeCopy, /*gap1050,*/ &isWrapped, isWrapped);
		// if (isWrapped)
		// 	sub34.dword0++;
		return shader;
	}
};

}
