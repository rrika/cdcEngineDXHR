#pragma once
#include <algorithm> // for clamp
#include "PCDX11InternalResource.h"
#include "PCDX11IndexBuffer.h"

namespace cdc {

class PCDX11VertexBuffer;
class PCDX11PixelShader;
class PCDX11VertexShader;

class PCDX11StateManager : public PCDX11InternalResource {
	ID3D11DeviceContext *m_deviceContext; // 10
	bool m_dirtyRasterizerState; // 19
	bool m_dirtyDepthStencilState; // 1A
	bool m_dirtyBlendState; // 1B
	bool m_dirtyConstantBuffers; // 1C
	int m_topology; // B8

	bool m_dirtyShaderResources; // CE
	bool m_dirtySamplers; // CF
	ID3D11SamplerState *m_samplers[16 + 4]; // D0
	ID3D11ShaderResourceView *m_resources[16 + 4]; // 120
	uint8_t m_dirtyShaderResourcesFirst; // 184
	uint8_t m_dirtyShaderResourcesLast; // 185
	uint8_t m_dirtySamplersFirst; // 186
	uint8_t m_dirtySamplersLast; // 187

	ID3D11Buffer *m_indexBufferD3D; // 188
	ID3D11Buffer *m_vertexBufferD3D; // 18C
	uint32_t m_vertexStride; // 190
	PCDX11PixelShader *m_pixelShader; // 198
	PCDX11VertexShader *m_vertexShader; // 19C
public:
	PCDX11StateManager();
	PCDX11StateManager(ID3D11DeviceContext *deviceContext) :
		m_deviceContext(deviceContext),
		m_indexBufferD3D(nullptr),
		m_pixelShader(nullptr)
	{}

	void setIndexBuffer(PCDX11IndexBuffer *indexBuffer);
	void setVertexBuffer(PCDX11VertexBuffer *vertexBuffer);
	void setPixelShader(PCDX11PixelShader *pixelShader);
	void setVertexShader(PCDX11VertexShader *vertexShader);
	void setPrimitiveTopology(int topology);

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
