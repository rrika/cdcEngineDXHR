#pragma once
#include "CommonDepthBuffer.h"
#include "PCDX11RenderTexture.h"
#include <d3d11.h>

namespace cdc {

class PCDX11RenderDevice;

class PCDX11DepthBuffer : public CommonDepthBuffer {
public:
	uint32_t flagsC = 0;
	bool isLocked = true;
	PCDX11RenderTexture renderTexture; // 18

	PCDX11DepthBuffer(
		uint32_t width, uint32_t height,
		uint32_t unknown, uint32_t format,
		PCDX11RenderDevice *renderDevice);

	TextureMap *getRenderTexture() override { return &renderTexture; };
	uint32_t getWidth() override { return renderTexture.getWidth(); };
	uint32_t getHeight() override { return renderTexture.getHeight(); };
	void EvictNow() override {
		if ((flagsC & 4) == 0)
			renderTexture.resFree();
	};
	void Evict(uint32_t passIndex) override { /*TODO*/ };
	void method_14() override { /*TODO*/ };

	void copyFromTexture(IDepthBuffer*) { /* empty, see call from PCDX11Scene::draw */ }

	ID3D11DepthStencilView *getRWDepthStencilView() {
		return static_cast<ID3D11DepthStencilView*>(renderTexture.getView());
	}
	ID3D11DepthStencilView *getRODepthStencilView() {
		return renderTexture.createDepthStencilView();
	}
};

}
