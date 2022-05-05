#pragma once
#include <algorithm> // for clamp
#include "PCDX11InternalResource.h"
#include "PCDX11IndexBuffer.h"

namespace cdc {

class PCDX11VertexBuffer;
class PCDX11PixelShader;
class PCDX11VertexShader;
class PCDX11BaseTexture;

class PCDX11StateManager : public PCDX11InternalResource {
	ID3D11DeviceContext *m_deviceContext; // 10
	ID3D11Device *m_device; // 14
	bool m_dirtyRasterizerState; // 19
	bool m_dirtyDepthStencilState; // 1A
	bool m_dirtyBlendState; // 1B
	bool m_dirtyConstantBuffers; // 1C
	int m_topology; // B8

	bool m_dirtyShaderResources; // CE
	bool m_dirtySamplers; // CF
	ID3D11SamplerState *m_samplers[16 + 4]; // D0
	ID3D11ShaderResourceView *m_textureViews[16 + 4]; // 120

	uint8_t m_dirtyShaderResourcesFirst; // 184
	uint8_t m_dirtyShaderResourcesLast; // 185
	uint8_t m_dirtySamplersFirst; // 186
	uint8_t m_dirtySamplersLast; // 187

	ID3D11Buffer *m_indexBufferD3D; // 188
	ID3D11Buffer *m_vertexBufferD3D; // 18C
	uint32_t m_vertexStride; // 190
	PCDX11PixelShader *m_pixelShader; // 198
	PCDX11VertexShader *m_vertexShader; // 19C

	PCDX11BaseTexture *m_textures[16 + 4]; // 1A8
	ID3D11Resource *m_textureResources[16 + 4]; // 1F8

	uint32_t m_samplerFilter[16 + 4]; // 248
	uint32_t m_samplerRepeat[16 + 4]; // 298

public:
	PCDX11StateManager();
	PCDX11StateManager(ID3D11DeviceContext *deviceContext, ID3D11Device *device) :
		m_deviceContext(deviceContext),
		m_device(device),
		m_indexBufferD3D(nullptr),
		m_pixelShader(nullptr),
		m_dirtySamplersFirst(20),
		m_dirtySamplersLast(0)
	{}

	void setIndexBuffer(PCDX11IndexBuffer *indexBuffer);
	void setVertexBuffer(PCDX11VertexBuffer *vertexBuffer);
	void setPixelShader(PCDX11PixelShader *pixelShader);
	void setVertexShader(PCDX11VertexShader *vertexShader);
	void setPrimitiveTopology(int topology);

	void setSamplerState(uint32_t slot, PCDX11BaseTexture *tex, uint32_t filter);
	void setTextureAndSampler(uint32_t slot, PCDX11BaseTexture *tex, uint32_t filter, float unknown);

	void updateRasterizerState();
	void updateDepthStencilState();
	void updateBlendState();
	void updateShaderResources();
	void updateSamplers();
	void updateConstantBuffers();

	void updateRenderTargets();
	void updateRenderState();

	void internalResource04() override;
	void internalResource08() override;
};

}
