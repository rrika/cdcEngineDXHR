#pragma once
#include <cstdint>
#include "PPPass.h"
#include "cdc/dtp/postprocessing.h"

namespace cdc {

class CommonRenderTarget;
struct RenderViewport;

}

class PPPrePass {
public:
	uint32_t dword4 = 0;
	PPTexture *textures = nullptr; // 8
	uint32_t numTextures = 0; // C
	PPVariable *variables = nullptr; // 10
	uint32_t numVariables = 0; // 14
	uint8_t byte18 = 0;
	uint8_t byte19 = 0;

	virtual ~PPPrePass() = default;

	void init(dtp::PPPrePassBlob *blob, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables);
	void createScene(PPRTs *rts, cdc::RenderViewport *viewport, cdc::CommonRenderTarget *rt);
};
