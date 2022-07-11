#pragma once
#include <cstdint>

struct PPTextureBlob {
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t width;
	uint32_t height;
	uint32_t dword14;
	uint32_t dword18;
};

class PPTexture {
	cdc::PCDX11RenderTarget *renderTarget; // 4
	PPTextureBlob blob; // 8
	uint32_t dword24;

public:
	virtual ~PPTexture() = default;
};
