#pragma once
#include "Stencil.h"

namespace cdc {

class CommonStreamDecl;
class IShaderLib;
class TextureMap;
struct ShaderInputSpec;

struct MaterialTexRef { // = cdc::MaterialData::TextureEntry
	TextureMap *tex;
	float unknown4; // = m_mipLodBias
	uint32_t dword8; // = m_category
	uint8_t fallbackIndex; // = m_type | m_class << 5
	uint8_t slotIndex;
	uint16_t filter;
};

struct MaterialBlobSub { // = cdc::PassData
	IShaderLib *shaderPixel;
	IShaderLib *shaderVertex;
	IShaderLib *shaderHull;
	IShaderLib *shaderDomain;
	uint32_t dword10;

	uint8_t psByte14;
	uint8_t psRefIndexEndB;
	uint8_t psRefIndexEndA;
	uint8_t psRefIndexBeginB;
	MaterialTexRef *psTextureRef;
	uint32_t psBufferSize;
	char *psBufferData;

	uint8_t vsByte24;
	uint8_t vsRefIndexEndB; // 25
	uint8_t vsRefIndexEndA; // 26
	uint8_t vsRefIndexBeginB; // 27
	MaterialTexRef *vsTextureRef; // 28
	uint32_t vsBufferSize; // 2C
	char *vsBufferData; // 30

	uint8_t psBufferFirstRow; // 34
	uint8_t psBufferNumRows; // 35

	uint8_t vsBufferFirstRow; // 36
	uint8_t vsBufferNumRows; // 37

	ShaderInputSpec *vsLayout[8]; // 38
};

struct MaterialBlob {

	// unconfirmed
	// enum FogType { // 77
	// 	kFogNone = 0,
	// 	kFogLinearDist = 1,
	// 	kFogNonLinearDist = 2,
	// 	kFogNonLinearDistAndHeight = 3
	// };

	enum FadeMode { // 85
		kFadeOff = 0,
		kFadeAlphaBlend = 1,
		kFadeAlphaTest = 2,
		kFadeAlphaToMask = 3,
		kFadeAlphaToMaskDither = 4
	};

	uint16_t word0;
	uint16_t word2;
	int dword4;
	int renderTargetWriteMask;
	int blendStateC; // for submat 0, 1, 5
	int blendFactors;
	uint16_t word14;
	uint16_t fadeMode; // 16
	int dword18; // flags, bit 11 controls renderTwice in ModelDrawable
	int dword1C;
	int dword20;
	uint32_t blendState24; // for submat 4
	uint8_t byte_28;
	uint8_t alphaThreshold; // 29
	uint8_t byte_2A;
	uint8_t byte_2B;
	int negDepthBias;
	float negSlopeScaledDepthBias;
	StencilParams stencilSettings34;
	int name_44;
	int dword48;
	MaterialBlobSub *subMat4C[16];
};

struct MaterialInstanceData {
	// see also PCDX11Material::setupPixelResources
	TextureMap *pInstanceTextures[4] = {}; // = pInstanceTextures (TextureMap*[5])
	float opacity = 1.0f; // 10
	uint32_t polyFlags = 0; // 14
	// 0x20000000: see PCDX11Material::setupDepthBias
	uint32_t m_updateTextureMask = 0;
	float minDepth = 0.0f; // = m_depthBoundsMin
	float maxDepth = 1.0f; // = m_depthBoundsMax
	CommonStreamDecl *streamDecls24[16] = {}; // = m_apPassStreamDecls
	StencilParams *stencilSettings64; // = m_pStencilParams
};

}
