#include "PPBuiltins.h"
#include "PPManager.h"
#include "PPPrePass.h"
#include "PPTexture.h"
#include "rendering/CommonRenderDevice.h"
#include "rendering/CommonScene.h"
#include "rendering/surfaces/PCDX11DefaultRenderTarget.h"

bool PPPrePass::init(dtp::PPPrePassBlob *blob, PPTexture *textures, uint32_t numTextures, PPVariable *variables, uint32_t numVariables) {
	this->blob = blob;
	this->textures = textures;
	this->numTextures = numTextures;
	this->variables = variables;
	this->numVariables = numVariables;
	this->writesRT0 = false;
	this->readsRT0 = false;

	if (textures[blob->outputTextureIndex].dword24 == 1)
		writesRT0 = true;

	for (uint32_t i=0; i<8; i++)
		if (blob->textureIndices[i] != ~0u)
			if (textures[blob->textureIndices[i]].dword24 == 1)
				readsRT0 = true;

	return blob->outputTextureIndex != ~0u;
}

void PPPrePass::run(PPRTs *rts, cdc::RenderViewport *viewport, cdc::CommonRenderTarget *fallback) {
	if (!blob->enabled)
		return;

	cdc::RenderViewport v = *viewport;
	v.clearMode = 0;
	v.byteC9 = 0;
	cdc::CommonRenderTarget *rt = textures[blob->outputTextureIndex].getRenderTarget();
	cdc::CommonRenderTarget *sourceColor = nullptr;

	if (blob->clearMode == 0) {
		v.clearMode = 1;

	} else if (blob->clearMode == 1) {
		v.clearMode = 2;
		float clearColorF[4] = {
			float(((blob->clearColor >> 16) & 0xff) / 255.0),
			float(((blob->clearColor >>  8) & 0xff) / 255.0),
			float(((blob->clearColor >>  0) & 0xff) / 255.0),
			float(((blob->clearColor >> 24) & 0xff) / 255.0)
		};
		v.clearColor[0] = clearColorF[0] * blob->clearColorScale;
		v.clearColor[1] = clearColorF[1] * blob->clearColorScale;
		v.clearColor[2] = clearColorF[2] * blob->clearColorScale;
		v.clearColor[3] = clearColorF[3] * blob->clearColorScale;

	} else if (blob->clearMode == 2) {
		v.clearMode = 1;
		sourceColor = textures[blob->sourceTextureIndex].getRenderTarget();
	}

	// TODO
	v.mask &= ~0x3000;

	static_cast<cdc::PCDX11RenderTarget*>(rt)->getTextureResource(); // HACK: ensure rendertargetview is created

	cdc::CommonRenderDevice *renderDevice = cdc::g_renderDevice;
	auto *subscene = renderDevice->createSubScene(
		&v,
		rt ? rt : fallback,
		blob->useDepthBuffer ? rts->db : nullptr,
		sourceColor,
		nullptr);
	subscene->debugName = std::string("prepass ") + std::string(blob->name);

	cdc::TextureMap *rt0 = textures[blob->textureIndices[0]].getRenderTexture(rts);

	if (blob->builtinShaderType) {
		switch (blob->builtinShaderType) {
		case 1:
			PPFastBlur(rt0, rt, 8, 1, /*0, 29,*/ 0); break; // horizontal un-weighted
		case 2:
			PPFastBlur(rt0, rt, 8, 0, /*0, 29,*/ 0); break; // vertical un-weighted
		case 3:
			PPFastBlur(rt0, rt, 8, 1, /*0, 29,*/ 1); break; // horizontal weighted
		case 4:
			PPFastBlur(rt0, rt, 8, 0, /*0, 29,*/ 1); break; // vertical weighted
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

