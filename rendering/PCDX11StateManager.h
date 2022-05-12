#pragma once
#include "PCDX11InternalResource.h"
#include "../matrix.h"

namespace cdc {

class PCDX11BaseTexture;
class PCDX11ConstantBuffer;
class PCDX11DepthBuffer;
class PCDX11IndexBuffer;
class PCDX11PixelShader;
class PCDX11RenderTarget;
class PCDX11StreamDecl;
class PCDX11VertexBuffer;
class PCDX11VertexShader;
class PCDX11UberConstantBuffer;

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
	PCDX11StreamDecl *m_streamDecl; // 194
	PCDX11PixelShader *m_pixelShader; // 198
	PCDX11VertexShader *m_vertexShader; // 19C

	PCDX11BaseTexture *m_textures[16 + 4]; // 1A8
	ID3D11Resource *m_textureResources[16 + 4]; // 1F8

	uint32_t m_samplerFilter[16 + 4]; // 248
	uint32_t m_samplerRepeat[16 + 4]; // 298

	float m_materialOpacity; // 300
	float m_fogColor[4]; // 310

	PCDX11ConstantBuffer *m_constantBufferVs[7]; // 330
	PCDX11ConstantBuffer *m_constantBufferPs[7]; // 34C

public:
	PCDX11RenderTarget *m_renderTarget = nullptr; // 380
	PCDX11DepthBuffer *m_depthBuffer = nullptr; // 384

private:
	uint32_t m_renderTargetStackIndex = 0; // 38C
	PCDX11RenderTarget *m_renderTargetStack[20]; // 390
	PCDX11DepthBuffer *m_depthBufferStack[20]; // 3E0

	float m_alphaThreshold; // 438

	float4x4 m_projectMatrix; // 440
	float4x4 m_viewMatrix; // 480
	float4x4 m_viewProjectMatrix; // 4C0
	float4x4 m_worldMatrix; // 500
	float4x4 *m_projectMatrixPtr; // 540
	bool m_dirtyWorldMatrix; // 544
	bool m_dirtyViewMatrix; // 545
	bool m_dirtyProjectMatrix; // 546

	PCDX11UberConstantBuffer *m_uberConstantBuffer[7]; // 5A8
	bool m_dirtyUberCBs[7]; // 5C4

public:
	PCDX11StateManager();
	PCDX11StateManager(ID3D11DeviceContext *deviceContext, ID3D11Device *device);

	void setIndexBuffer(PCDX11IndexBuffer *indexBuffer);
	void setVertexBuffer(PCDX11VertexBuffer *vertexBuffer);
	void setStreamDecl(PCDX11StreamDecl *streamDecl);
	void setPixelShader(PCDX11PixelShader *pixelShader);
	void setVertexShader(PCDX11VertexShader *vertexShader);
	void setPrimitiveTopology(int topology);

	void setSamplerState(uint32_t slot, PCDX11BaseTexture *tex, uint32_t filter);
	void setTextureAndSampler(uint32_t slot, PCDX11BaseTexture *tex, uint32_t filter, float unknown);
	void setPsConstantBuffer(uint32_t slot, PCDX11ConstantBuffer *cb);
	void setVsConstantBuffer(uint32_t slot, PCDX11ConstantBuffer *cb);
	void setCommonConstantBuffers();
	PCDX11UberConstantBuffer& accessCommonCB(uint32_t i);

	void setAlphaThreshold(uint8_t threshold);
	void setFogColor(float *color);
	void setOpacity(float opacity);
	void setWorldMatrix(float4x4& worldMatrix);
	void setViewMatrix(float4x4& viewMatrix);
	void setProjectMatrix(float4x4& projectMatrix);
	void setProjectMatrixPtr(float4x4* projectMatrixPtr);

	void updateMatrices();

	void updateRasterizerState();
	void updateDepthStencilState();
	void updateBlendState();
	void updateShaderResources();
	void updateSamplers();
	void updateConstantBuffers();

	void pushRenderTargets(PCDX11RenderTarget *renderTarget, PCDX11DepthBuffer *depthBuffer);
	void popRenderTargets();

	void updateRenderTargets(PCDX11RenderTarget *renderTarget, PCDX11DepthBuffer *depthBuffer);
	void updateRenderState();
	void updateViewport();

	bool internalCreate() override;
	void internalRelease() override;
};

}
