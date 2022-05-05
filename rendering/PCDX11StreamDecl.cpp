#include "PCDX11StreamDecl.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"

namespace cdc {

void PCDX11StreamDecl::apply() {
	auto deviceContext = renderDevice->getD3DDeviceContext();
	if (inputLayout)
		deviceContext->IASetInputLayout(inputLayout);
}

void PCDX11StreamDecl::internalResource04() {
	/*return*/ deviceManager->getD3DDevice()->CreateInputLayout(
		elementDesc,
		numElements,
		shaderSub->shaderBinary,
		shaderSub->shaderBinarySize,
		&inputLayout
	) == 0;
}

void PCDX11StreamDecl::internalResource08() {
	if (inputLayout) {
		inputLayout->Release();
		inputLayout = nullptr;
	}
}

}
