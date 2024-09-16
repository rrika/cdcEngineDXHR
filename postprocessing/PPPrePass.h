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
	dtp::PPPrePassBlob *blob = 0; // 4
	PPTexture *textures = nullptr; // 8
	uint32_t numTextures = 0; // C
	PPVariable *variables = nullptr; // 10
	uint32_t numVariables = 0; // 14
	bool writesRT0 = false; // 18
	bool readsRT0 = false; // 19

	virtual ~PPPrePass() = default;

	bool init(dtp::PPPrePassBlob *blob, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables);
	void run(PPRTs *rts, cdc::RenderViewport *viewport, cdc::CommonRenderTarget *rt);
};
