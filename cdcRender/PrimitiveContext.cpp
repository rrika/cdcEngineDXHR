#include "PrimitiveContext.h"
#include "cdcRender/CommonRenderDevice.h"

namespace cdc {

PrimitiveContext::PrimitiveContext(bool isTransient, CommonRenderDevice *renderDevice) {
	m_dirtyBits = isTransient ? 0xffff : 0;
	m_pReadState = nullptr;
	m_frameIndex = 0xffffffff;
	m_passes = 0;
	m_isTransient = isTransient;
	m_pRenderDevice = renderDevice;
	if (isTransient) {
		m_pWriteState = new (renderDevice, 5 /*kAllocQueue*/) State;
		m_pReadState = m_pWriteState;
		m_frameIndex = renderDevice->getFrameIndex();
	} else {
		m_pWriteState = (State*)renderDevice->InternalAlloc(sizeof(State));
	}
	m_pWriteState->m_flags = 0;
	m_pWriteState->m_depthBoundsMin = 0.0;
	m_pWriteState->m_depthBoundsMax = 1.0;
	m_pWriteState->m_pMaterial = nullptr;
	m_pWriteState->vertexBuffer = nullptr;
	m_pWriteState->m_pVertexDecl = nullptr;
	m_pWriteState->indexBuffer = nullptr;
	m_pWriteState->instanceParams = nullptr;
	m_pWriteState->scaleformData = nullptr;
	m_pWriteState->m_pInstanceTextures = nullptr;
	m_pWriteState->m_pWorldMatrix = nullptr;
	m_pWriteState->m_pProjectionOverrideMatrix = nullptr;
	m_pWriteState->m_pStencilParams = nullptr;
	m_pWriteState->ptr34_x10 = nullptr;
}

void PrimitiveContext::NewState() {
	// TODO
}

void PrimitiveContext::SetVertexDecl(VertexDecl *vd) {
	if (m_isTransient && m_dirtyBits == 0)
		NewState();
	m_dirtyBits |= 1;
	m_pWriteState->m_pVertexDecl = vd;
}

void PrimitiveContext::SetVertexBuffer(CommonVertexBuffer *vb) {
	if (m_isTransient && m_dirtyBits == 0)
		NewState();
	m_dirtyBits |= 1;
	m_pWriteState->vertexBuffer = vb;
}

void PrimitiveContext::SetIndexBuffer(CommonIndexBuffer *ib) {
	// TODO: call method on ib (maybe)
	if (m_isTransient && m_dirtyBits == 0)
		NewState();
	m_dirtyBits |= 1;
	m_pWriteState->indexBuffer = ib;
}

void PrimitiveContext::SetInstanceParam(uint32_t, Vector4Arg) {
	// TODO
}

void PrimitiveContext::SetInstanceTexture(uint32_t, TextureMap*) {
	// TODO
}

void PrimitiveContext::SetWorldMatrix(Matrix const&) {
	// TODO
}

}
