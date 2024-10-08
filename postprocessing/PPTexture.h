#pragma once
#include <cstdint>
#include "cdc/dtp/postprocessing.h"

namespace cdc { class CommonRenderTarget; class TextureMap; }

struct PPRTs;

class PPTexture {
public:
	cdc::CommonRenderTarget *renderTarget; // 4
	dtp::PPTextureBlob blob; // 8
	uint32_t dword24 = 0;

	void init(dtp::PPTextureBlob *blob);
	cdc::CommonRenderTarget *getRenderTarget();
	cdc::TextureMap *getRenderTexture(PPRTs *rts);
	void freeRenderTarget();

	virtual ~PPTexture() = default;
};
