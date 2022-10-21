#pragma once
#include <cstdint>
#include "cdc/dtp/postprocessing.h"

namespace cdc { class CommonRenderTarget; }

class PPTexture {
public:
	cdc::CommonRenderTarget *renderTarget; // 4
	dtp::PPTextureBlob blob; // 8
	uint32_t dword24;

	virtual ~PPTexture() = default;
};
