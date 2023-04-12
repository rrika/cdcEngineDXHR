#pragma once
#include "PCDX11RenderTarget.h"

namespace cdc {

class PCDX11RenderDevice;

class PCDX11SubFrameRenderTarget :
	public PCDX11RenderTarget
{
	PCDX11RenderDevice *renderDevice;
	uint32_t width8;
	uint32_t heightC;
	uint32_t width10;
	uint32_t height14;
	PCDX11RenderTarget *renderTarget;

public:
	PCDX11SubFrameRenderTarget(
		uint32_t a, uint32_t b, uint32_t c, uint32_t d,
		PCDX11RenderDevice *renderDevice,
		PCDX11RenderTarget *renderTarget)
	:
		renderDevice(renderDevice),
		width8(a), heightC(b),
		width10(c), height14(d),
		renderTarget(renderTarget)
	{}

	PCDX11RenderTexture *getRenderTexture() override { return renderTarget->getRenderTexture(); }
	uint32_t getWidth() override { return width10; }
	uint32_t getHeight() override { return height14; }
	void freeResource() override { /*empty*/ }
	void registerAtScene(void *) override { /*empty*/ }
	void method_14() override { /*empty*/ }
	// inherit bool method_18() { return false; };
	// inherit void method_1C() {};

	uint32_t method_24(uint32_t, uint32_t) override { return 0; }
	uint32_t method_28() override { return width8; }
	uint32_t method_2C() override { return heightC; }
	ID3D11Resource *getTextureResource() override { return renderTarget->getTextureResource(); }
	ID3D11RenderTargetView *getRenderTargetView() override { return renderTarget->getRenderTargetView(); }
	void copyFromTexture(void *ptr) override { return renderTarget->copyFromTexture(ptr); }
	bool method_3C() override { return renderTarget->method_3C(); }
	void method_40() override { renderTarget->method_40(); }
};

}
