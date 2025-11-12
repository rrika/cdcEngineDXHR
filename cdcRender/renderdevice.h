#pragma once
#include <cstdint>

namespace cdc {

struct StencilSettings;

struct RenderVertex { // line 294
	float vX, vY, vZ; uint32_t nDiffuse;
	float vU, vV;     uint32_t pad[2];
};

struct RenderVertexShadedDiffuse { // line 323
	float vX, vY, vZ; uint32_t diffuse;
	float vU, vV;     float nNX, nNY;
	float nNZ;        uint32_t pad[3];
};

struct LineVertex { // line 336
	float vX, vY, vZ;
	uint32_t nDiffuse;
};

enum PCRendererType { // line 1755
	RENDERER_DX9 = 0,
	RENDERER_DX11 = 1
};

// not to be confused with MaterialInstanceData
struct MaterialInstanceParams { // line 491
	cdc::TextureMap *m_pTextures[4] = {0, 0, 0, 0}; // 0
	float m_depthBoundsMin = 0.0f; // 10
	float m_depthBoundsMax = 1.0f; // 14
	StencilSettings *m_pStencilParams = nullptr; // 18
	cdc::Vector4 m_shaderConstants[8]; // 20
};

extern PCRendererType g_CurrentRenderer; // 1761

};
