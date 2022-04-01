#pragma once
#include "CommonMaterial.h"
#include "PCDX11BitmapTexture.h"
#include "PCDX11StaticConstantBuffer.h"

namespace cdc {

class PCDX11Material :
	public CommonMaterial
	// public PCDX11RenderExternalResource
{
	unsigned short numTextures = 0;
	unsigned short texBits = 0;
	// PCDX11BitmapTexture *texture[4];
	// PCDX11StaticConstantBuffer *constantBuffersPs[16];
	// PCDX11StaticConstantBuffer *constantBuffersVs[16];
public:
	PCDX11Material() {}

	void method_04() override;
	void method_0C() override;
	void method_10() override;
	~PCDX11Material() = default;
	void method_18() override;
};

}
