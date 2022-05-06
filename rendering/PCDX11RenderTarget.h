#pragma once
#include "CommonRenderTarget.h"
#include "PCDX11RenderTexture.h"

namespace cdc {

class PCDX11RenderTarget : public CommonRenderTarget {
public:
	PCDX11RenderTexture renderTexture; // 1C

	void getRenderTexture() override { /*TODO*/ };
	void getWidth() override { /*TODO*/ };
	void getHeight() override { /*TODO*/ };
	void method_0C() override { /*TODO*/ };
	void method_10() override { /*TODO*/ };
	void method_14() override { /*TODO*/ };

	virtual void method_24();
	virtual uint32_t method_28();
	virtual uint32_t method_2C();
	virtual ID3D11Resource *getTextureResource();
	virtual ID3D11RenderTargetView *getRenderTargetView();
	virtual void method_38();
	virtual void method_3C();
	virtual void method_40();
	virtual void method_44();
};

}
