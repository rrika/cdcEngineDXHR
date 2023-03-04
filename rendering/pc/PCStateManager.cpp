#include <d3d9.h>
#include "PCStateManager.h"
#include "shaders/PCPixelShader.h"
#include "shaders/PCVertexShader.h"

namespace cdc {

PCStateManager::PCStateManager() {}

void PCStateManager::setIndexBuffer(PCIndexBuffer *indexBuffer) {
	IDirect3DIndexBuffer9 *buffer = nullptr;
	if (indexBuffer)
		buffer = indexBuffer->getD3DBuffer();

	if (buffer != m_indexBufferD3D) {
		m_device->SetIndices(buffer);
		m_indexBufferD3D = buffer;
	}
}

void PCStateManager::setPixelShader(PCPixelShader *pixelShader) {
	if (pixelShader != m_pixelShader) {
		if (pixelShader) {
			if (!pixelShader->m_d3dShader)
				pixelShader->requestShader();
			m_device->SetPixelShader(pixelShader->m_d3dShader);
		} else {
			m_device->SetPixelShader(nullptr);
		}

		m_pixelShader = pixelShader;
	}
}

void PCStateManager::setVertexShader(PCVertexShader *vertexShader) {
	if (vertexShader != m_vertexShader) {
		if (vertexShader) {
			if (!vertexShader->m_d3dShader)
				vertexShader->requestShader();
			m_device->SetVertexShader(vertexShader->m_d3dShader);
		} else {
			m_device->SetVertexShader(nullptr);
		}
		m_vertexShader = vertexShader;
	}
}

bool PCStateManager::internalCreate() { return true; }; // TODO
void PCStateManager::internalRelease() {}; // TODO

}
