#pragma once
#include <cstdint>

namespace cdc {

struct RenderVertex { // line 294
	float vX, vY, vZ; uint32_t nDiffuse;
	float vU, vV;     uint32_t pad[2];
};

enum PCRendererType { // line 1755
	RENDERER_DX9 = 0,
	RENDERER_DX11 = 1
};

extern PCRendererType g_CurrentRenderer; // 1761

};
