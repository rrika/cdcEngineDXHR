#pragma once
#include <cstdint>
#include "cdc/dtp/postprocessing.h"

namespace cdc {

class CommonRenderTarget;
struct RenderViewport;

}

class PPTexture;
class PPVariable;
struct PPRTs;

class PPPass {
public:
	dtp::PPPassBlob *blob = nullptr;
	PPTexture *textures = nullptr; // 8
	uint32_t numTextures = 0; // C
	PPVariable *variables = nullptr; // 10
	uint32_t numVariables = 0; // 14
	bool readsRT0 = false; // 18

	virtual ~PPPass() = default;

	bool init(dtp::PPPassBlob *a2, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables);
	void run(cdc::CommonRenderTarget**, PPRTs *rts, cdc::RenderViewport *viewport, uint32_t pppassMask, bool, uint32_t texturesMask);
};
