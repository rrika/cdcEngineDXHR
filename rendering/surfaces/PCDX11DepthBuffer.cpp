#include "PCDX11DepthBuffer.h"

namespace cdc {

PCDX11DepthBuffer::PCDX11DepthBuffer() {
	uint32_t format = 0; // TODO
	renderTexture.isDepthBuffer = true;
	renderTexture.initForRenderTarget(this, format, nullptr);
}

}
