#include "PCDX11RenderTexture.h"
#include "PCDX11DeviceManager.h"
#include <d3d11.h>

namespace cdc {

void PCDX11RenderTexture::ensureRenderTargetView() {
	// TODO
}

void PCDX11RenderTexture::ensureBuffer() {
	// TODO
}

ID3D11View *PCDX11RenderTexture::getView() {
	return view;
}

void PCDX11RenderTexture::resRegisterAtRenderDevice() {
	// TODO
}

void PCDX11RenderTexture::resFill(void* src, size_t size, size_t offset) {
	// TODO
}

char *PCDX11RenderTexture::resGetBuffer() {
	// TODO
	return nullptr;
}

void PCDX11RenderTexture::resConstruct() {
	// TODO
}

ID3D11Resource *PCDX11RenderTexture::getTextureResource() {
	ensureBuffer();
	return resource;
}

ID3D11ShaderResourceView *PCDX11RenderTexture::createShaderResourceView() {
	ensureBuffer();
	return shaderResourceView;
}

ID3D11UnorderedAccessView *PCDX11RenderTexture::createUnorderedAccessView() {
	ensureBuffer();
	if (!unorderedAccessView) {
		// TODO
	}
	return unorderedAccessView;
}

}
