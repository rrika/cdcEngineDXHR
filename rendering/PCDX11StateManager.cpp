#define NOMINMAX
#include <d3d11_1.h>
#include <climits>
#include <algorithm>
#include "PCDX11BaseTexture.h"
#include "PCDX11ConstantBuffer.h"
#include "PCDX11DepthBuffer.h"
#include "PCDX11IndexBuffer.h"
#include "PCDX11PixelShader.h"
#include "PCDX11RenderTarget.h"
#include "PCDX11StateManager.h"
#include "PCDX11StreamDecl.h"
#include "PCDX11VertexBuffer.h"
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

void PCDX11StateManager::setStreamDecl(PCDX11StreamDecl *streamDecl) {
	if (streamDecl != m_streamDecl) {
		streamDecl->apply();
		m_streamDecl = streamDecl;
	}
}

void PCDX11StateManager::setPixelShader(PCDX11PixelShader *pixelShader) {
	if (pixelShader != m_pixelShader) {
		if (pixelShader) {
			if (!pixelShader->m_requested)
				pixelShader->requestShader();
			if (pixelShader->m_keepWaiting)
				pixelShader->awaitResource();
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
				vertexShader->awaitResource();
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

void PCDX11StateManager::setSamplerState(
	uint32_t slot,
	PCDX11BaseTexture *tex,
	uint32_t filter)
{
	// 0..15 => 0..15
	//   257 => 16
	//   258 => 17
	//   259 => 18
	//   260 => 19
	if (slot >= 0x10) slot -= 0xF1;

	uint32_t repeat = tex->repeatmode;

	if (repeat != m_samplerRepeat[slot] || filter != m_samplerFilter[slot]) {
		m_samplerRepeat[slot] = repeat;
		m_samplerFilter[slot] = filter;

		uint32_t maxAnisotropy = 1;
		D3D11_FILTER d3dfilter; 
		switch (filter) {
			case 1:
				d3dfilter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				break;
			case 2:
				d3dfilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				break;
			case 3: // 1
			case 4: // 2
			case 6: // 4
			case 10: // 8
			case 18: // 16
				d3dfilter = D3D11_FILTER_ANISOTROPIC;
				maxAnisotropy = filter - 2;
				break;
			default:
				d3dfilter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				break;
		}

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = d3dfilter;
		samplerDesc.AddressU = (repeat & 1) ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = (repeat & 2) ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = (repeat & 4) ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0;
		samplerDesc.MaxAnisotropy = maxAnisotropy;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;

		if (slot == 14) {
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
			samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		}

		// TODO: cache samplers
		ID3D11SamplerState *samplerState;
		m_device->CreateSamplerState(&samplerDesc, &samplerState);

		if (samplerState != m_samplers[slot]) {
			m_dirtySamplersFirst = std::min<uint8_t>(m_dirtySamplersFirst, slot);
			m_dirtySamplersLast = std::max<uint8_t>(m_dirtySamplersLast, slot);
			m_dirtySamplers = true;
			m_samplers[slot] = samplerState;
		}
	}
}

void PCDX11StateManager::setTextureAndSampler(
	uint32_t slot,
	PCDX11BaseTexture *tex,
	uint32_t filter,
	float unknown)
{
	// 0..15 => 0..15
	//   257 => 16
	//   258 => 17
	//   259 => 18
	//   260 => 19
	uint32_t originalSlot = slot;
	if (slot >= 0x10) slot -= 0xF1;
	if (!m_device) return;

	// TODO: logic to avoid binding a texture currently used as a rendertarget
	if (tex != m_textures[slot]) {
		if (auto srv = tex->createShaderResourceView()) {
			m_textureViews[slot] = srv;
			if (filter > tex->maxFilter)
				filter = tex->maxFilter;
			setSamplerState(originalSlot, tex, filter);
		}
		m_textureResources[slot] = tex->getTextureResource();
		m_dirtyShaderResourcesFirst = std::min<uint8_t>(m_dirtyShaderResourcesFirst, slot);
		m_dirtyShaderResourcesLast = std::max<uint8_t>(m_dirtyShaderResourcesLast, slot);
		m_dirtyShaderResources = true;
		m_textures[slot] = tex;
	}
}

void PCDX11StateManager::setPsConstantBuffer(uint32_t slot, PCDX11ConstantBuffer *cb) {
	if (cb != m_constantBufferPs[slot]) {
		ID3D11Buffer *buffer = cb->getBuffer();
		m_deviceContext->PSSetConstantBuffers(slot, 1, &buffer);
		m_constantBufferPs[slot] = cb;
	}
}

void PCDX11StateManager::setVsConstantBuffer(uint32_t slot, PCDX11ConstantBuffer *cb) {
	if (cb != m_constantBufferVs[slot]) {
		ID3D11Buffer *buffer = cb->getBuffer();
		m_deviceContext->VSSetConstantBuffers(slot, 1, &buffer);
		m_constantBufferVs[slot] = cb;
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
			psBegin, psEnd-psBegin, &m_textureViews[psBegin]);

	unsigned vsBegin = std::clamp(begin, 16u, 20u);
	unsigned vsEnd = std::clamp(end, 16u, 20u);

	if (vsBegin < vsEnd)
		m_deviceContext->VSSetShaderResources(
			vsBegin-16, vsEnd-vsBegin, &m_textureViews[vsBegin]);

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

void PCDX11StateManager::updateRenderTargets(
	PCDX11RenderTarget *renderTarget,
	PCDX11DepthBuffer *depthBuffer
) {
	// TODO
	m_renderTarget = renderTarget;
	m_depthBuffer = depthBuffer;
	ID3D11RenderTargetView *renderTargetView = renderTarget->getRenderTargetView();
	ID3D11DepthStencilView *depthStencilView = depthBuffer->getDepthStencilView();
	m_deviceContext->OMSetRenderTargets(
		!!renderTargetView,
		&renderTargetView,
		depthStencilView);
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

void PCDX11StateManager::updateViewport() {
	// TODO
}

bool PCDX11StateManager::internalCreate() { return true; };
void PCDX11StateManager::internalRelease() {};

}
