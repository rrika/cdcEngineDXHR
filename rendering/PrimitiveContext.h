#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc {

class CommonIndexBuffer;
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
};

}
