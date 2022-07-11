#pragma once
#include <cstdint>
#include "PPPass.h"

namespace cdc {

class CommonRenderTarget;
struct CommonSceneSub10;

}

using PPPrePassBlob = PPUnknown4;

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

	void init(PPPrePassBlob *blob, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables);
	void createScene(RTs *rts, cdc::CommonSceneSub10 *srcSub18, cdc::CommonRenderTarget *rt);
};
