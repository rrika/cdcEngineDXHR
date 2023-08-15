#pragma once
#include "cdcMath/Math.h"
#include "PCInternalResource.h"
#include "buffers/PCIndexBuffer.h"
#include "rendering/Types.h"

typedef enum _D3DRENDERSTATETYPE D3DRENDERSTATETYPE;

namespace cdc {

class PCDeviceBaseTexture;
class PCPixelShader;
class PCStreamDecl;
class PCVertexBuffer;
class PCVertexShader;

class PCStateManager : public PCInternalResource {
	IDirect3DDevice9 *m_device; // C
	IDirect3DIndexBuffer9 *m_indexBufferD3D; // 14
	PCVertexBuffer *m_vertexBuffer; // 18
	PCStreamDecl *m_streamDecl; // 1C
	PCPixelShader *m_pixelShader; // 20
	PCVertexShader *m_vertexShader; // 24
	PCDeviceBaseTexture *m_textures[20]; // 28

	uint32_t m_renderStates[210]; // 168, indexed by D3DRENDERSTATETYPE

	Matrix m_projectionMatrix; // 5B0
	Matrix m_viewMatrix; // 5F0, or perhaps m_invViewMatrix
	Matrix m_viewProjectMatrix; // 630
	Matrix m_worldMatrix; // 670
	Matrix *m_pProjectionOverrideMatrix = nullptr; // 6B0
	bool m_viewOrProjectMatrixIsDirty = false; // 6B4
	bool m_worldMatrixIsDirty = false;

public:
	PCStateManager();
	PCStateManager(IDirect3DDevice9 *device) :
		m_device(device),
		m_indexBufferD3D(nullptr)
	{}

	void setIndexBuffer(PCIndexBuffer *indexBuffer);
	void setDeviceTexture(uint32_t, PCDeviceBaseTexture*, TextureFilter, float);
	void setVertexBuffer(PCVertexBuffer *vertexBuffer);
	void setStreamDecl(PCStreamDecl *streamDecl);
	void setPixelShader(PCPixelShader *pixelShader);
	void setVertexShader(PCVertexShader *vertexShader);

	void SetVertexShaderConstantF(uint32_t index, float *data, uint32_t rows);
	void SetPixelShaderConstantF(uint32_t index, float *data, uint32_t rows);

	void SetWorldMatrix(Matrix *world);
	void SetViewMatrix(Matrix *view);
	void SetProjectionMatrix(Matrix *project);

	void UpdateStateMatrices();
	void SetRenderState(D3DRENDERSTATETYPE state, uint32_t value);

	bool internalCreate() override;
	void internalRelease() override;
};

}
