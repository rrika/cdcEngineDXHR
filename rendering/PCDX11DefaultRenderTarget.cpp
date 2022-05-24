#include "PCDX11DefaultRenderTarget.h"
#include <d3d11.h>

namespace cdc {

void PCDX11DefaultRenderTarget::method_24() {
	// TODO
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

void PCDX11DefaultRenderTarget::method_38() {
	// TODO
}

void PCDX11DefaultRenderTarget::method_3C() {
	// TODO
}

void PCDX11DefaultRenderTarget::method_40() {
	// TODO
}

void PCDX11DefaultRenderTarget::method_44() {
	// TODO
}

}
