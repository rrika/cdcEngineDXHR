#include <d3d11_1.h>
#include "PCDX11StateManager.h"
#include "PCDX11VertexBuffer.h"
#include "PCDX11PixelShader.h"
#include "PCDX11VertexShader.h"

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

void PCDX11StateManager::setVertexBuffer(PCDX11VertexBuffer *vertexBuffer) {
	if (vertexBuffer == nullptr)
		return;

	ID3D11Buffer *buffer = vertexBuffer->getD3DBuffer();
	uint32_t stride = vertexBuffer->getStride();
	uint32_t offset = 0;
	if (buffer != m_vertexBufferD3D && stride != m_vertexStride) {
		vertexBuffer->method14();
		m_deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
		m_vertexBufferD3D = buffer;
		m_vertexStride = stride;
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

void PCDX11StateManager::setVertexShader(PCDX11VertexShader *vertexShader) {
	if (vertexShader != m_vertexShader) {
		if (vertexShader) {
			if (!vertexShader->m_requested)
				vertexShader->requestShader();
			if (vertexShader->m_keepWaiting)
				vertexShader->await();
			m_deviceContext->VSSetShader(vertexShader->m_d3dShader, nullptr, 0);
		} else {
			m_deviceContext->VSSetShader(nullptr, nullptr, 0);
		}
		m_vertexShader = vertexShader;
	}
}

void PCDX11StateManager::setPrimitiveTopology(int topology) {
	// TODO: tesselation stuff
	if (topology != m_topology) {
		m_topology = topology;
		m_deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
	}
}

void PCDX11StateManager::updateRasterizerState() {
	// TODO
}

void PCDX11StateManager::updateDepthStencilState() {
	// TODO
}

void PCDX11StateManager::updateBlendState() {
	// TODO
}

void PCDX11StateManager::updateShaderResources() {
	unsigned begin = m_dirtyShaderResourcesFirst;
	unsigned end = m_dirtyShaderResourcesLast + 1;

	unsigned psBegin = std::clamp(begin, 0u, 16u);
	unsigned psEnd = std::clamp(end, 0u, 16u);

	if (psBegin < psEnd)
		m_deviceContext->PSSetShaderResources(
			psBegin, psEnd-psBegin, &m_resources[psBegin]);

	unsigned vsBegin = std::clamp(begin, 16u, 20u);
	unsigned vsEnd = std::clamp(end, 16u, 20u);

	if (vsBegin < vsEnd)
		m_deviceContext->VSSetShaderResources(
			vsBegin-16, vsEnd-vsBegin, &m_resources[vsBegin]);

	m_dirtyShaderResources = false;
	m_dirtyShaderResourcesFirst = 20;
	m_dirtyShaderResourcesLast = 0;
}

void PCDX11StateManager::updateSamplers() {
	unsigned begin = m_dirtySamplersFirst;
	unsigned end = m_dirtySamplersLast + 1;

	unsigned psBegin = std::clamp(begin, 0u, 16u);
	unsigned psEnd = std::clamp(end, 0u, 16u);

	if (psBegin < psEnd)
		m_deviceContext->PSSetSamplers(
			psBegin, psEnd-psBegin, &m_samplers[psBegin]);

	unsigned vsBegin = std::clamp(begin, 16u, 20u);
	unsigned vsEnd = std::clamp(end, 16u, 20u);

	if (vsBegin < vsEnd)
		m_deviceContext->VSSetSamplers(
			vsBegin-16, vsEnd-vsBegin, &m_samplers[vsBegin]);

	m_dirtySamplers = false;
	m_dirtySamplersFirst = 20;
	m_dirtySamplersLast = 0;
}

void PCDX11StateManager::updateConstantBuffers() {
	// TODO
}

void PCDX11StateManager::updateRenderTargets() {
	// TODO
}

void PCDX11StateManager::updateRenderState() {
	if (m_dirtyRasterizerState)
		updateRasterizerState();
	if (m_dirtyDepthStencilState)
		updateDepthStencilState();
	if (m_dirtyBlendState)
		updateBlendState();
	if (m_dirtyShaderResources)
		updateShaderResources();
	if (m_dirtySamplers)
		updateSamplers();
	if (m_dirtyConstantBuffers)
		updateConstantBuffers();
}


void PCDX11StateManager::internalResource04() {};
void PCDX11StateManager::internalResource08() {};

}
