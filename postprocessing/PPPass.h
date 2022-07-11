#pragma once
#include <cstdint>

namespace cdc {

class CommonDepthBuffer;
class CommonRenderTarget;
struct CommonSceneSub10;

}

class PPTexture;
class PPVariable;
struct PPUnknown4;

struct RTs {
	cdc::CommonRenderTarget *rt0;
	cdc::CommonDepthBuffer *db;
	cdc::CommonRenderTarget *rt8;
	cdc::CommonRenderTarget *rtC;
};

class PPPass {
public:
	uint32_t dword4 = 0;
	PPTexture *textures = nullptr; // 8
	uint32_t numTextures = 0; // C
	PPVariable *variables = nullptr; // 10
	uint32_t numVariables = 0; // 14
	uint8_t byte18 = 0;

	virtual ~PPPass() = default;

	void init(PPUnknown4 *a2, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables);
	void createScene(cdc::CommonRenderTarget*, RTs *rts, cdc::CommonSceneSub10 *a4, uint32_t pppassMask, bool, uint32_t texturesMask);
};
