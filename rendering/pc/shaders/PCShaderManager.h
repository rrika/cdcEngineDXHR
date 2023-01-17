#pragma once
#include "PCVertexShader.h"
#include "PCPixelShader.h"

namespace cdc {

class PCShaderManager {
	// TODO
public:
	PCPixelShader *createPixelShader(char *blob, bool takeCopy, bool isWrapped) {
		// TODO
		auto *shader = PCPixelShader::create(blob, takeCopy, /*TODO,*/ &isWrapped, isWrapped);
		// TODO
		return shader;
	}

	PCVertexShader *createVertexShader(char *blob, bool takeCopy, bool isWrapped) {
		// TODO
		auto *shader = PCVertexShader::create(blob, takeCopy, /*TODO,*/ &isWrapped, isWrapped);
		// TODO
		return shader;
	}
};

}
