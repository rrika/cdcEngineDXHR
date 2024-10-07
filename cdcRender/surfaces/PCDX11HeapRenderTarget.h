#pragma once
#include <cstdint>
#include "PCDX11DefaultRenderTarget.h"

namespace cdc {

class PCDX11HeapRenderTarget :
	public PCDX11DefaultRenderTarget
{
public:
	PCDX11HeapRenderTarget(
		uint32_t width, uint32_t height,
		uint32_t flags, uint32_t format,
		PCDX11RenderDevice *renderDevice, TextureClass textureClass);

	// inherit getRenderTexture
	// inherit getWidth
	// inherit getHeight
	// inherit freeResource
	// inherit registerAtScene
	void method_14() override;

	// inherit method_24
	// inherit method_28
	// inherit method_2C
	// inherit getTextureResource
	// inherit getRenderTargetView
	// inherit copyFromTexture
	// inherit method_3C
	// inherit method_40
};

}
