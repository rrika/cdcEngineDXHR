#pragma once
#include "../Types.h"
#include "PCDX11RenderTarget.h"
#include "PCDX11RenderTexture.h"
#include "PCDX11TextureUpdateCallback.h"

namespace cdc {

class PCDX11DefaultRenderTarget :
	public PCDX11RenderTarget,
	public PCDX11TextureUpdateCallback
{
public:
	PCDX11DefaultRenderTarget(
		uint32_t width, uint32_t height,
		uint32_t, uint32_t,
		PCDX11RenderDevice*, ID3D11Texture2D*, TextureClass);

	PCDX11RenderDevice *renderDevice; // 4, used for copy, renderTexture also has a renderDevice member
	uint32_t flags14;
	PCDX11RenderTexture renderTexture; // 1C

	PCDX11RenderTexture *getRenderTexture() override { return &renderTexture; }
	uint32_t getWidth() override { return renderTexture.getWidth(); }
	uint32_t getHeight() override { return renderTexture.getHeight(); }
	void EvictNow() override {
		if ((flags14 & 4) == 0)
			renderTexture.resFree();
	}
	void Evict(uint32_t passIndex) override { /*TODO*/ }
	void method_14() override { /*empty*/ }

	uint32_t method_24(uint32_t, uint32_t) override;
	uint32_t method_28() override;
	uint32_t method_2C() override;
	ID3D11Resource *getTextureResource() override;
	ID3D11RenderTargetView *getRenderTargetView() override;
	void copyFromTexture(PCDX11RenderTarget *) override;
	bool method_3C() override;
	void method_40() override;
};

}
