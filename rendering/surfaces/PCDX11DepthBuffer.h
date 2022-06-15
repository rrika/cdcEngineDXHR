#pragma once
#include "CommonDepthBuffer.h"
#include "PCDX11RenderTexture.h"
#include <d3d11.h>

namespace cdc {

class PCDX11DepthBuffer : public CommonDepthBuffer {
public:
	PCDX11RenderTexture renderTexture; // 18

	PCDX11DepthBuffer();

	TextureMap *getRenderTexture() override { return &renderTexture; };
	uint32_t getWidth() override { return renderTexture.getWidth(); };
	uint32_t getHeight() override { return renderTexture.getHeight(); };
	void registerAtRenderDevice() override { /*TODO*/ };
	void registerAtScene(void *ptr) override { /*TODO*/ };
	void method_14() override { /*TODO*/ };

	ID3D11DepthStencilView *getDepthStencilView() {
		return static_cast<ID3D11DepthStencilView*>(renderTexture.getView());
	}
};

}
