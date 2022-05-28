#pragma once
#include "PCDX11DefaultRenderTarget.h"

namespace cdc {

class PCDX11HeapRenderTarget :
	public PCDX11DefaultRenderTarget
{
public:
	TextureMap *getRenderTexture() override { /*TODO*/ }
	uint32_t getWidth() override { /*TODO*/ }
	uint32_t getHeight() override { /*TODO*/ }
	void method_0C() override { /*TODO*/ }
	void method_10() override { /*TODO*/ }
	void method_14() override { /*TODO*/ }

	void method_24() override { /*TODO*/ }
	uint32_t method_28() override { /*TODO*/ }
	uint32_t method_2C() override { /*TODO*/ }
	ID3D11Resource *getTextureResource() override { /*TODO*/ }
	ID3D11RenderTargetView *getRenderTargetView() override { /*TODO*/ }
	void method_38() override { /*TODO*/ }
	void method_3C() override { /*TODO*/ }
	void method_40() override { /*TODO*/ }
	void method_44() override { /*TODO*/ }
};

}
