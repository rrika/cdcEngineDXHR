#include <cstring>
#include "PPPass.h"
#include "PPTexture.h"
#include "PPVariable.h"

bool PPPass::init(dtp::PPPassBlob *blob, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables) {
	uint32_t variableIndices[24];
	uint32_t texturesIndices[8];
	memcpy(variableIndices, blob->variableIndices, sizeof(variableIndices));
	memcpy(texturesIndices, blob->texturesIndices, sizeof(texturesIndices));

	bool success = true;
	if (blob->material == nullptr && blob->scaleform == false && blob->builtinShaderType == 0)
		success = false;

	if (blob->canBeDisabledThroughSettings) {
		// if (...)
		// 	success = false;
	}

	// ensure texture indices are in range
	for (uint32_t i=0; i<8; i++)
		if (auto ti = texturesIndices[i]; ti != ~0u && ti >= numTextures) {
			success = false;
			break;
		}

	// ensure variable indices are in range
	for (uint32_t i=0; i<24; i++)
		if (auto vi = texturesIndices[i]; vi != ~0u && vi >= numVariables) {
			success = false;
			break;
		}

	this->blob = blob;
	this->textures = textures;
	this->numTextures = numTextures;
	this->variables = variables;
	this->numVariables = numVariables;
	this->byte18 = 0; // does any texture have dword24 == 1

	for (uint32_t i=0; i<8; i++)
		if (texturesIndices[i] == ~0u)
			if (textures[texturesIndices[i]].dword24 == 1)
				byte18 = 1;

	return success;
}

void PPPass::createScene(cdc::CommonRenderTarget*, RTs *rts, cdc::RenderViewport *a4, uint32_t pppassMask, bool, uint32_t texturesMask) {
	// TODO
}
