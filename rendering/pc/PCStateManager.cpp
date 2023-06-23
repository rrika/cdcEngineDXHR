#include <cstdio>
#include <d3d9.h>
#include "PCStateManager.h"
#include "PCStreamDecl.h"
#include "buffers/PCVertexBuffer.h"
#include "shaders/PCPixelShader.h"
#include "shaders/PCVertexShader.h"
#include "surfaces/PCDeviceBaseTexture.h"

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

void PCStateManager::setDeviceTexture(uint32_t slot, PCDeviceBaseTexture *tex, TextureFilter filter, float unknown) {
	if (tex != m_textures[slot]) {
		if (tex) {
			IDirect3DBaseTexture9 *d3dTexture = tex->GetD3DTexture();
			m_device->SetTexture(slot, d3dTexture);
		} else {
			m_device->SetTexture(slot, nullptr);
		}
		m_textures[slot] = tex;
	}
}

void PCStateManager::setVertexBuffer(PCVertexBuffer *vertexBuffer) {
	if (vertexBuffer != m_vertexBuffer) {
		if (vertexBuffer) {
			IDirect3DVertexBuffer9 *buffer = vertexBuffer->GetD3DVertexBuffer();
			uint16_t stride = vertexBuffer->GetStride();
			m_device->SetStreamSource(0, buffer, 0, stride);
		} else {
			m_device->SetStreamSource(0, nullptr, 0, 0);
		}
		m_vertexBuffer = vertexBuffer;
	}
}

void PCStateManager::setStreamDecl(PCStreamDecl *streamDecl) {
	if (streamDecl != m_streamDecl) {
		if (streamDecl)
			streamDecl->apply();
		m_streamDecl = streamDecl;
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
