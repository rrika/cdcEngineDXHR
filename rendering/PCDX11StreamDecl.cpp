#include "buffers/PCDX11VertexBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11StreamDecl.h"
#include <d3d11.h>

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

bool PCDX11StreamDecl::internalCreate() {
	return deviceManager->getD3DDevice()->CreateInputLayout(
		elementDesc,
		numElements,
		shaderSub->shaderBinary,
		shaderSub->shaderBinarySize,
		&inputLayout
	) == 0;
}

void PCDX11StreamDecl::internalRelease() {
	if (inputLayout) {
		inputLayout->Release();
		inputLayout = nullptr;
	}
}

}
