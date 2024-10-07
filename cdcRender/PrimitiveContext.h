#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc {

class CommonIndexBuffer;
class CommonRenderDevice;
class CommonVertexBuffer;
class IMaterial;
class TextureMap;
struct StencilParams;
struct VertexDecl;

class PrimitiveContext {
public:
	struct State {
		uint32_t m_flags; // 0
		float m_depthBoundsMin; // 4
		float m_depthBoundsMax; // 8
		IMaterial *m_pMaterial; // C
		CommonVertexBuffer *vertexBuffer; // 10
		VertexDecl *m_pVertexDecl; // 14
		CommonIndexBuffer *indexBuffer; // 18
		void *instanceParams; // 1C
		Vector *scaleformData; // 20
		TextureMap **m_pInstanceTextures; // 24
		Matrix *m_pWorldMatrix; // 28
		Matrix *m_pProjectionOverrideMatrix; // 2C
		StencilParams *m_pStencilParams; // 30
		void *ptr34_x10;

		const cdc::Matrix* GetWorldMatrix();
		bool HasShaders();
	};

	State *m_pWriteState;
	State *m_pReadState;
	uint32_t m_frameIndex;
	uint32_t m_passes;
	uint16_t m_dirtyBits;
	bool m_isTransient;
	CommonRenderDevice *m_pRenderDevice;

	PrimitiveContext(bool isTransient, CommonRenderDevice *renderDevice);

	void NewState();
	void SetVertexDecl(VertexDecl*);
	void SetVertexBuffer(CommonVertexBuffer*);
	void SetIndexBuffer(CommonIndexBuffer*);
	void SetInstanceParam(uint32_t, Vector4Arg);
	void SetInstanceTexture(uint32_t, TextureMap*);
	void SetWorldMatrix(Matrix const&);
};

}
