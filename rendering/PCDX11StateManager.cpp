#include <d3d11_1.h>
#include "PCDX11StateManager.h"

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

void PCDX11StateManager::internalResource04() {};
void PCDX11StateManager::internalResource08() {};

}
