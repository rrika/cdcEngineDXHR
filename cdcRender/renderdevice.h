#pragma once
#include <cstdint>

namespace cdc {

struct StencilSettings;

struct RenderVertex { // line 294
	float vX, vY, vZ; uint32_t nDiffuse;
	float vU, vV;     uint32_t pad[2];
};

enum PCRendererType { // line 1755
	RENDERER_DX9 = 0,
	RENDERER_DX11 = 1
};

// not to be confused with MaterialInstanceData
struct MaterialInstanceParams { // line 491
	cdc::TextureMap *m_pTextures[4]; // 0
	float m_depthBoundsMin; // 10
	float m_depthBoundsMax; // 14
	StencilSettings *m_pStencilParams; // 18
	cdc::Vector4 m_shaderConstants[8]; // 20
};

extern PCRendererType g_CurrentRenderer; // 1761

};
