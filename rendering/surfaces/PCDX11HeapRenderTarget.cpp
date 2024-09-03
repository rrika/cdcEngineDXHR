#include "PCDX11HeapRenderTarget.h"

namespace cdc {

PCDX11HeapRenderTarget::PCDX11HeapRenderTarget(
	uint32_t width, uint32_t height,
	uint32_t flags, uint32_t format,
	PCDX11RenderDevice *renderDevice, TextureClass textureClass)
:
	PCDX11DefaultRenderTarget(width, height, flags, format, renderDevice, nullptr, textureClass)
{}

void PCDX11HeapRenderTarget::method_14() { /*TODO*/ }

}
