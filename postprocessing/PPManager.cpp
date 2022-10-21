#include "PPManager.h"
#include "PPPass.h"

PPManager::PPManager() {
	activeSets.reserve(16);
	unknowns2.reserve(16);
	textures.reserve(16);
	variables.reserve(16);
}

bool PPManager::prepare() {
	// TODO
	return true;
}

bool PPManager::createScene(
	cdc::CommonRenderTarget*,
	cdc::CommonRenderTarget*,
	cdc::CommonDepthBuffer*,
	cdc::RenderViewport*)
{
	if (!prepare())
		return false;

	dtp::PPVarPassTexBlobs *vpt = activeSets[0]->varPassTex;
	dtp::PPPassBlob *passBlobs = vpt->passes;
	PPPass pass;
	for (uint32_t i=0; i < vpt->numPasses; i++) {
		pass.init(&passBlobs[i],
			textures.data(),
			textures.size(),
			variables.data(),
			variables.size());
		// TODO
	}
	// TODO
	return true;
}

void PPManager::addActiveSet(dtp::PPActiveSet *unk1, float f) {
	// TODO
}
