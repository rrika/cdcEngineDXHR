#include <d3d11_1.h>
#include "PCDX11StateManager.h"
#include "PCDX11PixelShader.h"

namespace cdc {

PCDX11StateManager::PCDX11StateManager() {}

void PCDX11StateManager::setIndexBuffer(PCDX11IndexBuffer *indexBuffer) {
	ID3D11Buffer *buffer = nullptr;
	if (indexBuffer)
		buffer = indexBuffer->getD3DBuffer();

	if (buffer != m_indexBufferD3D) {
		indexBuffer->method_10();
		m_deviceContext->IASetIndexBuffer(buffer, DXGI_FORMAT_R16_UINT, 0);
		m_indexBufferD3D = buffer;
	}
}

void PCDX11StateManager::setPixelShader(PCDX11PixelShader *pixelShader) {
	if (pixelShader != m_pixelShader) {
		if (pixelShader) {
			if (!pixelShader->m_requested)
				pixelShader->requestShader();
			if (pixelShader->m_keepWaiting)
				pixelShader->await();
			m_deviceContext->PSSetShader(pixelShader->m_d3dShader, nullptr, 0);
		} else {
			m_deviceContext->PSSetShader(nullptr, nullptr, 0);
		}
		m_pixelShader = pixelShader;
	}
}

void PCDX11StateManager::internalResource04() {};
void PCDX11StateManager::internalResource08() {};

}
