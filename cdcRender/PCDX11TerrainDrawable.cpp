#include "buffers/PCDX11StaticIndexBuffer.h"
#include "buffers/PCDX11VertexBuffer.h"
#include "CommonRenderTerrainInstance.h"
#include "PCDX11LightManager.h"
#include "PCDX11Material.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderTerrain.h"
#include "PCDX11StateManager.h"
#include "PCDX11TerrainDrawable.h"
#include "Types.h"

#include "config.h"
#ifdef ENABLE_IMGUI
#include "imgui/imgui.h"
#endif

namespace cdc {

PCDX11TerrainDrawable::PCDX11TerrainDrawable(
	RenderTerrainGroup *pGroup,
	TerrainChunk **ppGeoms,
	uint32_t numGeoms,
	uint32_t flags,
	PCDX11TerrainState *pState)
{
	// IRenderDrawable
	sortZ = 0.0f;
	typeID = kDrawableTypeIDTerrain; // 2
	order = 0;

	m_pTerrain = pState->renderTerrain; // HACK
	m_pInstance = pState->renderTerrainInstance;
	m_pGroup = pGroup;
	m_pInstData = &pState->m_pInstanceData[pGroup - m_pTerrain->m_pGroups];
	m_ppGeoms = ppGeoms;
	m_numGeoms = numGeoms;
	m_flags = flags | 0xC;
	m_pLocalToWorld = &pState->m_toWorld;
	m_pInstanceParams = pState->m_pInstanceParams;
}

void PCDX11TerrainDrawable::draw1(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);

	thisTerrain->setMatrix(prevTerrain);

	auto *material = static_cast<PCDX11Material*>(thisTerrain->m_pInstData->material);
	auto *streamDecl = material->SetupDepthPass(
		&thisTerrain->m_pInstData->matData,
		thisTerrain->m_pInstanceParams,
		0,
		false,
		thisTerrain->m_pTerrain->m_pVertexBuffers[thisTerrain->m_pGroup->vbIndex].pVertexDecl,
		thisTerrain->m_flags,
		1.0f,
		0.0f);

	thisTerrain->draw(streamDecl, false, false);
}

void PCDX11TerrainDrawable::draw2(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);

	thisTerrain->setMatrix(prevTerrain);

	auto *material = static_cast<PCDX11Material*>(thisTerrain->m_pInstData->material);
	auto *lightManager = static_cast<PCDX11LightManager*>(thisTerrain->m_pTerrain->renderDevice->lightManager);
	auto *streamDecl = material->SetupShadowPass(
		&thisTerrain->m_pInstData->matData,
		thisTerrain->m_pInstanceParams,
		0, // TODO: lightManager434_114
		0,
		thisTerrain->m_pTerrain->m_pVertexBuffers[thisTerrain->m_pGroup->vbIndex].pVertexDecl,
		thisTerrain->m_flags,
		1.0f,
		0.0f);

	thisTerrain->draw(streamDecl, false, false);
}

void PCDX11TerrainDrawable::draw7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);

	thisTerrain->setMatrix(prevTerrain);

	auto *material = static_cast<PCDX11Material*>(thisTerrain->m_pInstData->material);
	auto *streamDecl = material->SetupBloomPass(
		&thisTerrain->m_pInstData->matData,
		thisTerrain->m_pInstanceParams,
		0,
		thisTerrain->m_pTerrain->m_pVertexBuffers[thisTerrain->m_pGroup->vbIndex].pVertexDecl,
		thisTerrain->m_flags,
		0.0f);

	bool doubleSided = (material->materialBlob->dword18 >> 11) & 1;
	thisTerrain->draw(streamDecl, doubleSided, false);
}

void PCDX11TerrainDrawable::draw4(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);

	thisTerrain->setMatrix(prevTerrain);

	if ((thisTerrain->m_flags & 1) == 0)
		thisTerrain->buildAndAssignLightBuffer();

	auto *material = static_cast<PCDX11Material*>(thisTerrain->m_pInstData->material);
	auto *streamDecl = material->SetupSinglePass(
		&thisTerrain->m_pInstData->matData,
		thisTerrain->m_pInstanceParams,
		0,
		false,
		thisTerrain->m_pTerrain->m_pVertexBuffers[thisTerrain->m_pGroup->vbIndex].pVertexDecl,
		thisTerrain->m_flags,
		false,
		1.0f,
		0.0f);

	thisTerrain->draw(streamDecl, false, true);
}

void PCDX11TerrainDrawable::draw56(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);

	thisTerrain->setMatrix(prevTerrain);

	if ((thisTerrain->m_flags & 1) == 0)
		thisTerrain->buildAndAssignLightBuffer();

	auto *material = static_cast<PCDX11Material*>(thisTerrain->m_pInstData->material);
	auto *streamDecl = material->SetupSinglePass(
		&thisTerrain->m_pInstData->matData,
		thisTerrain->m_pInstanceParams,
		thisTerrain->m_lightReceiverData,
		0,
		thisTerrain->m_pTerrain->m_pVertexBuffers[thisTerrain->m_pGroup->vbIndex].pVertexDecl,
		thisTerrain->m_flags,
		true,
		thisTerrain->m_opacity,
		0.0f);

	bool doubleSided = (material->materialBlob->dword18 >> 11) & 1;
	thisTerrain->draw(streamDecl, doubleSided, true);
}

void PCDX11TerrainDrawable::drawA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);

	thisTerrain->setMatrix(prevTerrain);

	auto *material = static_cast<PCDX11Material*>(thisTerrain->m_pInstData->material);
	auto *streamDecl = material->SetupNormalMapPass(
		&thisTerrain->m_pInstData->matData,
		thisTerrain->m_pInstanceParams,
		0,
		thisTerrain->m_pTerrain->m_pVertexBuffers[thisTerrain->m_pGroup->vbIndex].pVertexDecl,
		thisTerrain->m_flags,
		1.0f,
		0.0f);

	thisTerrain->draw(streamDecl, false, true);
}

bool PCDX11TerrainDrawable::compare12(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);
	// TODO
	return false;
}

bool PCDX11TerrainDrawable::compare467(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);
	// TODO
	return false;
}

bool PCDX11TerrainDrawable::compareA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisTerrain = static_cast<PCDX11TerrainDrawable*>(drawable);
	auto *prevTerrain = static_cast<PCDX11TerrainDrawable*>(prevDrawable);
	// TODO
	return false;
}


void PCDX11TerrainDrawable::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	// empty
}

uint32_t PCDX11TerrainDrawable::compare(uint32_t funcSetIndex, IRenderDrawable *other) {
	return 1;
}

void PCDX11TerrainDrawable::buildUI(uint32_t funcSetIndex, UIActions& uiact) {
#if ENABLE_IMGUI

	// copied from PCDX11ModelDrawable, might not be correct
	uint32_t subMatIndex = ~0u;
	switch (funcSetIndex) {
		case kRenderFunctionDepth:
			break; // not implemented
		case kRenderFunctionShadow:
			break; // not implemented
		case kRenderFunctionComposite:
			subMatIndex = 3; break;
		case kRenderFunctionTranslucent:
		case kRenderFunctionPredator:
			subMatIndex = 8; break;
		case kRenderFunctionAlphaBloomFSX:
			break; // not implemented
		case kRenderFunctionNormal:
			subMatIndex = 7; break;

		case kRenderFunctionDefault:
		case kRenderFunctionOpaque:
		case kRenderFunction8:
		case kRenderFunctionXRay:
			break;
	}

	MaterialBlob *materialData = nullptr;
	if (m_pInstData && m_pInstData->material)
		materialData = m_pInstData->material->materialBlob;
	MaterialBlobSub *subMat = nullptr;
	if (materialData && subMatIndex < 16)
		subMat = materialData->subMat4C[subMatIndex];

	if (ImGui::Button("Show")) {
		uiact.select(m_pTerrain);            // RenderTerrain*
		uiact.select(m_pInstData->material); // IMaterial*
		uiact.select(subMat);                // MaterialBlobSub*
		uiact.select(m_pTerrain->m_pVertexBuffers[m_pGroup->vbIndex].pVertexDecl); // VertexDecl*
	}
	ImGui::Text("TODO");
#endif
}

void PCDX11TerrainDrawable::draw(
	PCDX11StreamDecl *streamDecl,
	bool renderTwice,
	bool debugColors)
{
	if (!streamDecl)
		return;

	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	auto *renderDevice = m_pTerrain->renderDevice;
	RenderTerrainVertexBuffer *vb = &m_pTerrain->m_pVertexBuffers[m_pGroup->vbIndex];
	auto *vertexBuffer = static_cast<PCDX11VertexBuffer*>(vb->pPrimaryVertexBuffer);
	auto *indexBuffer = static_cast<PCDX11IndexBuffer*>(m_pTerrain->indexBuffer);
	stateManager->setVertexBuffer(vertexBuffer);
	stateManager->setStreamDecl(streamDecl);
	stateManager->setIndexBuffer(indexBuffer);

	for (uint32_t i=0; i<m_numGeoms; i++) {

		// TODO: accumulate chunks
		uint32_t indexCount = m_ppGeoms[i]->m_numIndices;
		uint32_t chunkStartIndex = m_ppGeoms[i]->m_startIndex;

		if (renderTwice) {
			// TODO
		}

		stateManager->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST /*4*/);
		stateManager->updateRenderState();
		uint32_t baseVertex = vertexBuffer->getBaseVertex();
		uint32_t bufferStartIndex = indexBuffer->getStartIndex();
		renderDevice->getD3DDeviceContext()->DrawIndexed(
			indexCount,
			chunkStartIndex + bufferStartIndex,
			baseVertex);

		if (renderTwice) {
			// TODO
		}
	}

	// TODO: return number of triangles drawn
}

bool PCDX11TerrainDrawable::setMatrix(IRenderDrawable *prevDrawable) {
	// TODO
	auto *stateManager = deviceManager->getStateManager();
	stateManager->setWorldMatrix(*m_pLocalToWorld);
	stateManager->updateMatrices();
	stateManager->setDepthLayer(m_pInstance ? m_pInstance->m_isDrawBehindAll : false);

	return false;
}

void PCDX11TerrainDrawable::buildAndAssignLightBuffer() {
	auto lightManager = static_cast<PCDX11LightManager*>(m_pTerrain->renderDevice->lightManager);
	// if (lightManager->lightDataX_E10 != m_lightReceiverData)
		lightManager->fillLightBuffer(m_lightReceiverData);

	if (m_lightConstantBufferData) {
		// if (lightManager->mostRecentAssignmentToCommonCB5 != m_lightConstantBufferData)
			lightManager->assignCommonCB5((char*)m_lightConstantBufferData);
	}
}

}
