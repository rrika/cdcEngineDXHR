#pragma once
#include "CommonDepthBuffer.h"
#include "PCDX11RenderTexture.h"
#include <d3d11.h>

namespace cdc {

class PCDX11DepthBuffer : public CommonDepthBuffer {
public:
	PCDX11RenderTexture renderTexture; // 18

	void getRenderTexture() override { /*TODO*/ };
	void getWidth() override { /*TODO*/ };
	void getHeight() override { /*TODO*/ };
	void method_0C() override { /*TODO*/ };
	void method_10() override { /*TODO*/ };
	void method_14() override { /*TODO*/ };

	ID3D11DepthStencilView *getDepthStencilView() {
		return static_cast<ID3D11DepthStencilView*>(renderTexture.getView());
	}
};

}
