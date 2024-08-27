#include <cstring>
#include "PPBuiltins.h"
#include "PPPass.h"
#include "PPPrePass.h"
#include "PPManager.h"
#include "PPTexture.h"
#include "PPVariable.h"
#include "rendering/CommonRenderDevice.h"
#include "rendering/CommonScene.h"

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

void PPPass::run(cdc::CommonRenderTarget **output, PPRTs *rts, cdc::RenderViewport *viewport, uint32_t pppassMask, bool isRootPass, uint32_t texturesMask) {
	// TODO
	if (blob->enabled && isRootPass || !blob->gatePrePasses) {
		for (uint32_t i = 0; i < blob->numPrePasses; i++) {
			if ((1 << i) & pppassMask) {
				PPPrePass prepass;
				if (prepass.init(
					&blob->prePasses[i],
					textures, numTextures,
					variables, numVariables))
				{
					if (blob->prePasses[i].enabled) {
						if (prepass.readsRT0 && !blob->prePasses[i].byte16) { // reads RT0
							if (*output != rts->rt0)
								std::swap(rts->rtC, rts->rt0);

						}
						if (prepass.writesRT0) // outputs to RT0
							*output = rts->rtC;

						prepass.run(rts, viewport, rts->rtC);
					}
				}
			}
		}
		if (blob->enabled && isRootPass) {

			if (readsRT0) {
				if (*output != rts->rt0)
					std::swap(rts->rt0, rts->rtC);

			} else if (blob->blendOverPrevious) {
				if (*output != rts->rtC)
					std::swap(rts->rt0, rts->rtC);
			}
			*output = rts->rtC;

			cdc::RenderViewport v = *viewport;
			v.clearMode = 0;
			v.byteC9 = 0;
			v.mask &= ~0x3000; // 0xFFEF8FFF

			// if (blob->byteE)
			//	v.clearMode = 1;

			cdc::CommonRenderDevice *renderDevice = cdc::g_renderDevice;

			auto *rt = rts->rtC;
			auto *subscene = renderDevice->createSubScene(
				&v,
				rt,
				blob->useDepthBuffer ? rts->db : nullptr,
				nullptr,
				nullptr);
			subscene->debugName = std::string("pass ") + std::string(blob->name);

			if (blob->builtinShaderType) {

				cdc::TextureMap *rt0 = textures[blob->textureIndices[0]].getRenderTexture(rts);

				pptrace.emplace_back(blob->name, 0, std::vector<uint32_t>{blob->textureIndices[0]});

				switch (blob->builtinShaderType) {
				case 1:
					PPFastBlur(rt0, rt, 8, 1, /*0, 29,*/ 0); break;
				case 2:
					PPFastBlur(rt0, rt, 8, 0, /*0, 29,*/ 0); break;
				case 3:
					PPFastBlur(rt0, rt, 8, 1, /*0, 29,*/ 1); break;
				case 4:
					PPFastBlur(rt0, rt, 8, 0, /*0, 29,*/ 1); break;
				case 5:
					PPAntiAlias(rt0, rt, 8); break;
				default:
					break;
				}

			} else if (blob->material) {
				cdc::MaterialInstanceParams mip;
				mip.m_pTextures[0] = blob->textureIndices[0] != ~0u ? textures[blob->textureIndices[0]].getRenderTexture(rts) : nullptr;
				mip.m_pTextures[1] = blob->textureIndices[1] != ~0u ? textures[blob->textureIndices[1]].getRenderTexture(rts) : nullptr;
				mip.m_pTextures[2] = blob->textureIndices[2] != ~0u ? textures[blob->textureIndices[2]].getRenderTexture(rts) : nullptr;
				mip.m_pTextures[3] = blob->textureIndices[3] != ~0u ? textures[blob->textureIndices[3]].getRenderTexture(rts) : nullptr;
				mip.m_depthBoundsMin = 0.0;
				mip.m_depthBoundsMax = 1.0;
				mip.m_pStencilParams = nullptr;

				std::vector<uint32_t> inputs;
				if (blob->blendOverPrevious)
					inputs.push_back(0);
				for (uint32_t i=0; i<4; i++)
					if (blob->textureIndices[i] != ~0u)
						inputs.push_back(blob->textureIndices[i]);
				pptrace.emplace_back(blob->name, 0, inputs);

				for (uint32_t i=0; i<8; i++)
					if (blob->variableIndices[i] != ~0u)
						mip.m_shaderConstants[i] = cdc::Vector4{variables[blob->variableIndices[i]].m_value};

				for (uint32_t i=8; i<24; i++)
					if (blob->variableIndices[i] != ~0u)
						renderDevice->SetGlobalParams(i-8, 1, variables[blob->variableIndices[i]].m_value.vec128);

				PPQuad(v.nearz + 10.0f, blob->material, &mip, 0.0f, /*primFlags=*/ 0x400, /*mask=*/ 8 /*0x42CB*/);
			}

			renderDevice->finishScene();
		}
	}
}
