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

	TextureMap *getRenderTexture() override { return &renderTexture; }
	uint32_t getWidth() override { return renderTexture.getWidth(); }
	uint32_t getHeight() override { return renderTexture.getHeight(); }
	void registerAtRenderDevice() override { /*TODO*/ }
	void registerAtScene(void *ptr) override { /*TODO*/ }
	void method_14() override { /*empty*/ }

	void method_24() override;
	uint32_t method_28() override;
	uint32_t method_2C() override;
	ID3D11Resource *getTextureResource() override;
	ID3D11RenderTargetView *getRenderTargetView() override;
	void copyFromTexture(void *) override;
	void method_3C() override;
	void method_40() override;
	void method_44() override;
};

}
