#pragma once

namespace cdc {

enum PCRendererType { // 1755
	RENDERER_DX9 = 0,
	RENDERER_DX11 = 1
};

extern PCRendererType g_CurrentRenderer; // 1761

};
