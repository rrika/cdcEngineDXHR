#pragma once
#include <cstdint>
#include "cdcSys/Array.h"
#include "cdc/dtp/postprocessing.h"
#include "PPTexture.h"
#include "PPVariable.h"

namespace cdc {

class CommonRenderTarget;
class CommonDepthBuffer;
struct RenderViewport;

}

struct PPUnknown2 {
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t dword20;
	uint32_t dword24;
	uint32_t dword28;
	uint32_t dword2C;
};

class PPManager {
public:
	Array<dtp::PPActiveSet*> activeSets; // 4
	Array<PPTexture> textures; // 10
	Array<PPVariable> variables; // 1C
	uint32_t rootPasses28;
	uint32_t dwords2C[32];
	uint32_t dwordsAC[32];
	uint32_t dwords12C[32];
	uint32_t width;
	uint32_t height;
	Array<PPUnknown2*> unknowns2; // 1B4

	PPManager();
	virtual ~PPManager() = default;

	bool prepare();
	bool createScene(
		cdc::CommonRenderTarget*,
		cdc::CommonRenderTarget*,
		cdc::CommonDepthBuffer*,
		cdc::RenderViewport*);
	void addActiveSet(dtp::PPActiveSet *unk1, float f);
};
