#pragma once
#include "PCDX11RenderTarget.h"
#include "PCDX11TextureUpdateCallback.h"

namespace cdc {

class PCDX11DefaultRenderTarget :
	public PCDX11RenderTarget,
	public PCDX11TextureUpdateCallback
{
public:
	PCDX11RenderTexture renderTexture; // 1C

	void getRenderTexture() override { /*TODO*/ };
	void getWidth() override { /*TODO*/ };
	void getHeight() override { /*TODO*/ };
	void method_0C() override { /*TODO*/ };
	void method_10() override { /*TODO*/ };
	void method_14() override { /*TODO*/ };

	void method_24() override;
	uint32_t method_28() override;
	uint32_t method_2C() override;
	ID3D11Resource *getTextureResource() override;
	ID3D11RenderTargetView *getRenderTargetView() override;
	void method_38() override;
	void method_3C() override;
	void method_40() override;
	void method_44() override;
};

}
