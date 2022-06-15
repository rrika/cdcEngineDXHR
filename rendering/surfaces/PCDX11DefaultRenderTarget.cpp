#include "PCDX11DefaultRenderTarget.h"
#include <d3d11.h>

namespace cdc {

PCDX11DefaultRenderTarget::PCDX11DefaultRenderTarget(
	uint32_t width, uint32_t height,
	uint32_t unknown1, uint32_t format,
	PCDX11RenderDevice *renderDevice, ID3D11Texture2D *texture, uint32_t unknown2)
{
	// TODO
	renderTexture.initForRenderTarget(this, format, texture);
}


uint32_t PCDX11DefaultRenderTarget::method_24(uint32_t, uint32_t) {
	// TODO
	return 0;
}

uint32_t PCDX11DefaultRenderTarget::method_28() {
	// TODO
	return 0;
}

uint32_t PCDX11DefaultRenderTarget::method_2C() {
	// TODO
	return 0;
}

ID3D11Resource *PCDX11DefaultRenderTarget::getTextureResource() {
	return renderTexture.getTextureResource();
}

ID3D11RenderTargetView *PCDX11DefaultRenderTarget::getRenderTargetView() {
	return static_cast<ID3D11RenderTargetView*>(renderTexture.getView());
}

void PCDX11DefaultRenderTarget::copyFromTexture(void *) {
	// TODO
}

bool PCDX11DefaultRenderTarget::method_3C() {
	// TODO
	return false;
}

void PCDX11DefaultRenderTarget::method_40() {
	// TODO
}

}
