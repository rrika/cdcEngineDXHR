#include <cstring>
#include "PPPass.h"
#include "PPTexture.h"
#include "PPVariable.h"

bool PPPass::init(dtp::PPPassBlob *blob, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables) {
	uint32_t variableIndices[24];
	uint32_t textureIndices[8];
	memcpy(variableIndices, blob->variableIndices, sizeof(variableIndices));
	memcpy(textureIndices, blob->textureIndices, sizeof(textureIndices));

	bool success = true;
	if (blob->material == nullptr && blob->scaleform == false && blob->builtinShaderType == 0)
		success = false;

	if (blob->canBeDisabledThroughSettings) {
		// if (...)
		// 	success = false;
	}

	// ensure texture indices are in range
	for (uint32_t i=0; i<8; i++)
		if (auto ti = textureIndices[i]; ti != ~0u && ti >= numTextures) {
			success = false;
			break;
		}

	// ensure variable indices are in range
	for (uint32_t i=0; i<24; i++)
		if (auto vi = variableIndices[i]; vi != ~0u && vi >= numVariables) {
			success = false;
			break;
		}

	this->blob = blob;
	this->textures = textures;
	this->numTextures = numTextures;
	this->variables = variables;
	this->numVariables = numVariables;
	this->readsRT0 = 0; // does any texture have dword24 == 1

	for (uint32_t i=0; i<8; i++)
		if (textureIndices[i] != ~0u)
			if (textures[textureIndices[i]].dword24 == 1)
				readsRT0 = 1;

	return success;
}

void PPPass::run(cdc::CommonRenderTarget **output, PPRTs *rts, cdc::RenderViewport *viewport, uint32_t pppassMask, bool, uint32_t texturesMask) {
	// TODO
}
