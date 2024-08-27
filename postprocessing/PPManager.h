#pragma once
#include <cstdint>
#include <vector>
#include "cdcSys/Array.h"
#include "cdc/dtp/postprocessing.h"
#include "PPTexture.h"
#include "PPVariable.h"

namespace cdc {

class CommonRenderTarget;
class CommonDepthBuffer;
class IMaterial;
struct MaterialInstanceParams;
struct RenderViewport;

}

struct UIActions;

void PPQuad(float depth, cdc::IMaterial *material, cdc::MaterialInstanceParams *instanceParams, float sortZ, uint32_t primFlags, uint32_t passMask);

struct PPRTs {
	cdc::CommonRenderTarget *rt0 = nullptr;
	cdc::CommonDepthBuffer *db = nullptr;
	cdc::CommonRenderTarget *rt8 = nullptr;
	cdc::CommonRenderTarget *rtC = nullptr;
};

struct PPTraceNode {
	const char *name;
	uint32_t output;
	std::vector<uint32_t> inputs;
};

extern std::vector<PPTraceNode> pptrace;

class PPManager {
public:
	static PPManager *s_instance;

	std::vector<dtp::PPActiveSet*> activeSets; // 4
	std::vector<PPTexture> textures; // 10
	std::vector<PPVariable> variables; // 1C
	uint32_t rootPasses; // 28
	uint32_t prePassMasks[32]; // 12C, one for each pass
	uint32_t width = 0;
	uint32_t height = 0;
	// std::vector<PPInput*> inputs; // 1B4

	// HACK
	bool showUnusedVariables = true;
	uint32_t variablesMask;
	dtp::PPVarPassTexBlobs *fallbackVarPassTex = nullptr;
	uint32_t rootOverride = 0;
	uint32_t rootOverrideMask = 0;

	PPManager();
	virtual ~PPManager() = default;

	bool prepare();
	bool run(
		cdc::CommonRenderTarget *rt,
		cdc::CommonRenderTarget *rtParticle,
		cdc::CommonRenderTarget *rtDest, // HACK
		cdc::CommonDepthBuffer *depth,
		cdc::RenderViewport *viewport);
	void resetActiveSets();
	void addActiveSet(dtp::PPActiveSet *activeSet, float f);

	void buildUI(UIActions& uiact);
};
