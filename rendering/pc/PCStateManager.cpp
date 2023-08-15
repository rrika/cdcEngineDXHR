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

void PCStateManager::SetVertexShaderConstantF(uint32_t index, float *data, uint32_t rows) {
	m_device->SetVertexShaderConstantF(index, data, rows);
}

void PCStateManager::SetPixelShaderConstantF(uint32_t index, float *data, uint32_t rows) {
	m_device->SetPixelShaderConstantF(index, data, rows);
}

void PCStateManager::SetWorldMatrix(Matrix *world) {
	m_worldMatrix = *world;
	m_worldMatrixIsDirty = true;
}

void PCStateManager::SetViewMatrix(Matrix *view) {
	m_viewMatrix = *view;
	m_viewOrProjectMatrixIsDirty = true;
}

void PCStateManager::SetProjectionMatrix(Matrix *project) {
	m_projectionMatrix = *project;
	m_viewOrProjectMatrixIsDirty = true;
}

void PCStateManager::UpdateStateMatrices() {
	if (m_viewOrProjectMatrixIsDirty) {
		Matrix *project = m_pProjectionOverrideMatrix;
		if (!project)
			project = &m_projectionMatrix;
		m_viewProjectMatrix = (*project) * m_viewMatrix;

		Matrix buffer[2];
		buffer[0] = m_viewMatrix;
		buffer[1] = m_viewProjectMatrix;
		m_device->SetVertexShaderConstantF(8, (float*)buffer, 8); // assign vs rows 8..15
		m_device->SetPixelShaderConstantF(8, (float*)buffer, 4); // assign ps rows 8..11
    }
	if (m_viewOrProjectMatrixIsDirty || m_worldMatrixIsDirty) {
		Matrix buffer[2];
		buffer[0] = m_viewProjectMatrix * m_worldMatrix;
		buffer[1] = m_worldMatrix;
		m_device->SetVertexShaderConstantF(0, (float*)buffer, 8); // assign vs rows 0..7
		m_viewOrProjectMatrixIsDirty = false;
		m_worldMatrixIsDirty = false;
	}
}

void PCStateManager::SetRenderState(D3DRENDERSTATETYPE state, uint32_t value) {
	if (m_renderStates[state] != value) {
		m_device->SetRenderState(state, value);
		m_renderStates[state] = value;
	}
}

bool PCStateManager::internalCreate() { return true; }; // TODO
void PCStateManager::internalRelease() {}; // TODO

}
