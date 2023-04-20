#include "PCDX11DepthBuffer.h"

namespace cdc {

PCDX11DepthBuffer::PCDX11DepthBuffer(
	uint32_t width, uint32_t height,
	uint32_t unknown, uint32_t format,
	PCDX11RenderDevice *renderDevice)
:
	renderTexture(width, height, unknown, /*isDepthBuffer=*/ true, renderDevice, kTextureClass2D)
{
	renderTexture.initForRenderTarget(this, format, nullptr);
}

}
