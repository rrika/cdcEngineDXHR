#include "PPPrePass.h"

bool PPPrePass::init(dtp::PPPrePassBlob *blob, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables) {
	this->blob = blob;
	// TODO
	return true;
}

void PPPrePass::run(PPRTs *rts, cdc::RenderViewport *viewport, cdc::CommonRenderTarget *rt) {
	// TODO
}

