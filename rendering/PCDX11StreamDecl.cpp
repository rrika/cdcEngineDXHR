#include "PCDX11StreamDecl.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11VertexBuffer.h"

namespace cdc {

void PCDX11StreamDecl::apply() {
	auto deviceContext = renderDevice->getD3DDeviceContext();
	if (inputLayout)
		deviceContext->IASetInputLayout(inputLayout);
	if (vertexBuffer) {
		ID3D11Buffer *buffer = vertexBuffer->getD3DBuffer();
		uint32_t stride = vertexBuffer->getStride();
		uint32_t offset = 0;
		deviceContext->IASetVertexBuffers(1, 1, &buffer, &stride, &offset);
	}
	// TODO: constant buffer
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
