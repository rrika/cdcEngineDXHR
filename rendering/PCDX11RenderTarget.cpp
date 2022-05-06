#include "PCDX11RenderTarget.h"
#include <d3d11.h>

namespace cdc {

void PCDX11RenderTarget::method_24() {
	// TODO
}

uint32_t PCDX11RenderTarget::method_28() {
	// TODO
	return 0;
}

uint32_t PCDX11RenderTarget::method_2C() {
	// TODO
	return 0;
}

ID3D11Resource *PCDX11RenderTarget::getTextureResource() {
	return renderTexture.getTextureResource();
}

ID3D11RenderTargetView *PCDX11RenderTarget::getRenderTargetView() {
	return static_cast<ID3D11RenderTargetView*>(renderTexture.getView());
}

void PCDX11RenderTarget::method_38() {
	// TODO
}

void PCDX11RenderTarget::method_3C() {
	// TODO
}

void PCDX11RenderTarget::method_40() {
	// TODO
}

void PCDX11RenderTarget::method_44() {
	// TODO
}

}
