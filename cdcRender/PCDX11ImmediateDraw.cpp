#include <cstdio>
#include "MaterialData.h"
#include "PCDX11ImmediateDraw.h"
#include "PCDX11Material.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11StateManager.h"
#include "buffers/PCDX11IndexBuffer.h"
#include "buffers/PCDX11VertexBuffer.h"

namespace cdc {

PrimitiveInfo::PrimitiveInfo(uint32_t polyFlags, uint32_t numPrims, bool hasSourceIndices) :
	m_polyFlags(polyFlags),
	m_numPrims(numPrims)
{
	switch ((polyFlags & POLYFLAG_PRIMTYPE) >> 16) {
	case /*0*/ POLYFLAG_TRILIST >> 16:
		m_numVertices = 3 * numPrims;
		m_d3dPrimType = 4; // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		break;
	case /*1*/ POLYFLAG_QUADLIST >> 16:
		m_numVertices = (hasSourceIndices ? 6 : 4) * numPrims;
		m_d3dPrimType = 4; // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		break;
	case /*2*/ POLYFLAG_TRISTRIP >> 16:
		m_numPrims = numPrims-2;
		m_numVertices = numPrims;
		m_d3dPrimType = 5; // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		break;
	case /*3*/ POLYFLAG_LINELIST >> 16: // HACK: not present in DXHR, taken from TRAS
		m_numVertices = 2 * numPrims;
		m_d3dPrimType = 2; // D3D11_PRIMITIVE_TOPOLOGY_LINELIST
		break;		
	default:
		break;
	}
}


PCDX11NGAPrimitive::PCDX11NGAPrimitive(
	PrimitiveContext::State *pState,
	PrimitiveInfo *primInfo,
	uint32_t startIndex,
	float sortKey,
	PCDX11Material *pMaterial,
	PCDX11RenderDevice *pRenderDevice)
:
	IRenderDrawable(),
	m_pRenderDevice(pRenderDevice),
	m_pState(pState),
	m_pMaterial(pMaterial),
	m_pLightSet(nullptr), // TODO
	m_pIrradianceState(nullptr), // TODO
	// m_pInstanceData(),
	m_numPrims(primInfo->m_numPrims),
	m_numVertices(primInfo->m_numVertices),
	m_startIndex(startIndex),
	topology(primInfo->m_d3dPrimType)
{
	typeID = kDrawableTypeIDNGAPrimitves;
	order = 0;
	sortZ = sortKey;

	// TODO

	m_pInstanceData = new (pRenderDevice) MaterialInstanceData;
	BuildInstanceDataAndFlags(*m_pInstanceData);
}

uint32_t PCDX11NGAPrimitive::BuildInstanceDataAndFlags(MaterialInstanceData& instanceData) {
	// MaterialInstanceData is in MaterialData.h
	for (uint32_t i=0; i<4; i++)
		instanceData.pInstanceTextures[i] =
			m_pState->m_pInstanceTextures ? m_pState->m_pInstanceTextures[i] : nullptr; // TODO

	instanceData.minDepth = m_pState->m_depthBoundsMin;
	instanceData.maxDepth = m_pState->m_depthBoundsMax;
	instanceData.stencilSettings64 = m_pState->m_pStencilParams;
	instanceData.polyFlags = m_pState->m_flags & (
		POLYFLAG_DOUBLESIDED |
		POLYFLAG_NOCOLOR |
		POLYFLAG_FADING |
		POLYFLAG_DEPTHBIAS
	); // 0x20000540

	uint32_t flags = m_pRenderDevice->getScene()->m_parity < 0.0f ? 2 : 0;
	return flags;
}

bool PCDX11NGAPrimitive::compare(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisNGA = static_cast<PCDX11NGAPrimitive*>(drawable);
	auto *prevNGA = static_cast<PCDX11NGAPrimitive*>(prevDrawable);
	if (thisNGA->m_pMaterial != prevNGA->m_pMaterial)
		return thisNGA->m_pMaterial < prevNGA->m_pMaterial;
	else
		return thisNGA->m_pState < prevNGA->m_pState;
}

void PCDX11NGAPrimitive::drawDepth(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisNGA = static_cast<PCDX11NGAPrimitive*>(drawable);
	auto *prevNGA = static_cast<PCDX11NGAPrimitive*>(prevDrawable);

	thisNGA->SetupTransform(prevNGA);

	uint32_t flags = thisNGA->m_pRenderDevice->scene78->m_parity < 0.0f ? 2 : 0;
	// auto *pMaterial = static_cast<PCDX11Material*>(thisNGA->m_pState->m_pMaterial);
	PCDX11StreamDecl *streamDecl = thisNGA->m_pMaterial->SetupDepthPass(
		thisNGA->m_pInstanceData,
		thisNGA->m_pState->instanceParams,
		/*vsSelect=*/ 0,
		false,
		thisNGA->m_pState->m_pVertexDecl,
		flags,
		/*opacity=*/ 1.0f,
		0.0f);
	thisNGA->DrawPrimitive(streamDecl, /*isBackBeforeFront=*/ false, flags);
}

void PCDX11NGAPrimitive::drawShadow(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisNGA = static_cast<PCDX11NGAPrimitive*>(drawable);
	auto *prevNGA = static_cast<PCDX11NGAPrimitive*>(prevDrawable);

	thisNGA->SetupTransform(prevNGA);

	// TODO
}

void PCDX11NGAPrimitive::drawAlphaBloom(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisNGA = static_cast<PCDX11NGAPrimitive*>(drawable);
	auto *prevNGA = static_cast<PCDX11NGAPrimitive*>(prevDrawable);

	thisNGA->SetupTransform(prevNGA);

	// TODO
}

void PCDX11NGAPrimitive::drawComposite(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisNGA = static_cast<PCDX11NGAPrimitive*>(drawable);
	auto *prevNGA = static_cast<PCDX11NGAPrimitive*>(prevDrawable);

	thisNGA->SetupTransform(prevNGA);

	uint32_t flags = thisNGA->m_pRenderDevice->scene78->m_parity < 0.0f ? 2 : 0;
	// auto *lightManager = static_cast<PCDX11LightManager*>(renderDevice->lightManager84);
	// if (m_pLightSet)
	// 	lightManager->fillLightBuffer(m_pLightSet);
	// if (m_pIrradianceState)
	// 	lightManager->assignCommonCB5(m_pIrradianceState);

	// auto *pMaterial = static_cast<PCDX11Material*>(thisNGA->m_pState->m_pMaterial);
	PCDX11StreamDecl *streamDecl = thisNGA->m_pMaterial->SetupSinglePass(
		thisNGA->m_pInstanceData,
		thisNGA->m_pState->instanceParams,
		thisNGA->m_pLightSet,
		/*vsSelect=*/ 0,
		thisNGA->m_pState->m_pVertexDecl,
		flags,
		/*isTranslucentPass=*/ false,
		/*opacity=*/ 1.0f,
		0.0f);
	thisNGA->DrawPrimitive(streamDecl, /*isBackBeforeFront=*/ false, flags);
}

void PCDX11NGAPrimitive::drawTranslucent(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisNGA = static_cast<PCDX11NGAPrimitive*>(drawable);
	auto *prevNGA = static_cast<PCDX11NGAPrimitive*>(prevDrawable);

	thisNGA->SetupTransform(prevNGA);

	uint32_t flags = thisNGA->m_pRenderDevice->scene78->m_parity < 0.0f ? 2 : 0;
	// auto *lightManager = static_cast<PCDX11LightManager*>(renderDevice->lightManager84);
	// if (m_pLightSet)
	// 	lightManager->fillLightBuffer(m_pLightSet);
	// if (m_pIrradianceState)
	// 	lightManager->assignCommonCB5(m_pIrradianceState);

	// auto *pMaterial = static_cast<PCDX11Material*>(thisNGA->m_pState->m_pMaterial);
	PCDX11StreamDecl *streamDecl = thisNGA->m_pMaterial->SetupSinglePass(
		thisNGA->m_pInstanceData,
		thisNGA->m_pState->instanceParams,
		thisNGA->m_pLightSet,
		/*vsSelect=*/ 0,
		thisNGA->m_pState->m_pVertexDecl,
		flags,
		/*isTranslucentPass=*/ true,
		/*opacity=*/ 1.0f,
		0.0f);
	thisNGA->DrawPrimitive(streamDecl, /*isBackBeforeFront=*/ false, flags);
}

void PCDX11NGAPrimitive::drawNormal(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisNGA = static_cast<PCDX11NGAPrimitive*>(drawable);
	auto *prevNGA = static_cast<PCDX11NGAPrimitive*>(prevDrawable);

	thisNGA->SetupTransform(prevNGA);

	uint32_t flags = thisNGA->m_pRenderDevice->scene78->m_parity < 0.0f ? 2 : 0;
	// auto *pMaterial = static_cast<PCDX11Material*>(thisNGA->m_pState->m_pMaterial);
	PCDX11StreamDecl *streamDecl = thisNGA->m_pMaterial->SetupNormalMapPass(
		thisNGA->m_pInstanceData,
		thisNGA->m_pState->instanceParams,
		/*vsSelect=*/ 0,
		thisNGA->m_pState->m_pVertexDecl,
		flags,
		/*opacity=*/ 1.0f,
		0.0f);
	uint32_t matDword18 = thisNGA->m_pMaterial->materialBlob->dword18;
	bool materialRenderTwice = bool(matDword18 & 0x800);
	thisNGA->DrawPrimitive(streamDecl, /*isBackBeforeFront=*/ materialRenderTwice, flags);
}


void PCDX11NGAPrimitive::draw(uint32_t funcSetIndex, IRenderDrawable *other) { // line 68
	return;
}

uint32_t PCDX11NGAPrimitive::compare(uint32_t funcSetIndex, IRenderDrawable *other) { // line 67
	return 1;
}


void PCDX11NGAPrimitive::DrawPrimitive(PCDX11StreamDecl *pStreamDecl, bool isBackBeforeFront, uint32_t flags) {
	if (!pStreamDecl)
		return;

	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	bool stereoOverride = false;
	// TODO

	auto *pVertexBuffer = static_cast<PCDX11VertexBuffer*>(m_pState->vertexBuffer);
	stateManager->setStreamDecl(pStreamDecl);
	stateManager->setVertexBuffer(pVertexBuffer);

	if ((m_pState->m_flags & /*0x100*/ POLYFLAG_NOCOLOR) != 0) {
		stateManager->setRenderTargetWriteMask(0); // depth-only?
		PCDX11Material::invalidate();
	}
	if (m_pState->scaleformData) {
		PCDX11UberConstantBuffer & cb3 = stateManager->accessCommonCB(3); // CB1 in VS
		cb3.assignRow(0, (void*)m_pState->scaleformData, 3); // 3 × float4
	}

	stateManager->setDepthLayer(false); // never in the sky

	if (isBackBeforeFront)
		stateManager->setCullMode(CullMode::front, (flags & 2) != 0);
	
	auto *d3d11DeviceContext = m_pRenderDevice->d3dDeviceContext111580;

	bool isQuadList = (m_pState->m_flags & /*0x30000*/ POLYFLAG_PRIMTYPE) == /*0x10000*/ POLYFLAG_QUADLIST;

	if (m_pState->indexBuffer || isQuadList) {
		uint32_t startIndex, maxBatchSize;
		if (m_pState->indexBuffer) {
			auto *pIndexBuffer = static_cast<PCDX11IndexBuffer*>(m_pState->indexBuffer);
			stateManager->setIndexBuffer(pIndexBuffer);
			startIndex = pIndexBuffer->getStartIndex() + m_startIndex;
			maxBatchSize = m_numPrims;
		} else {
			auto *pIndexBuffer = m_pRenderDevice->GetQuadsIndexBuffer();
			stateManager->setIndexBuffer(pIndexBuffer);
			startIndex = pIndexBuffer->getStartIndex();
			if (m_pState->m_flags & 0x800000)
				startIndex += 6000;
			maxBatchSize = 1000;
			// TODO: m_startIndex
		}

		uint32_t numPrimsLeft = m_numPrims;
		while (numPrimsLeft > 0) {
			uint32_t batchSize = numPrimsLeft > maxBatchSize ? maxBatchSize : numPrimsLeft;
			uint32_t indexCount = isQuadList ? 6 * batchSize : 3 * batchSize;
			uint32_t baseVertex = pVertexBuffer->getBaseVertex();

			stateManager->setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			stateManager->updateRenderState();
			d3d11DeviceContext->DrawIndexed(
				indexCount,
				startIndex,
				baseVertex);

			numPrimsLeft -= batchSize;
		}

		if (isBackBeforeFront) {
			// TODO
		}

	} else {
		stateManager->setPrimitiveTopology(topology);
		stateManager->updateRenderState();
		d3d11DeviceContext->Draw(
			m_numVertices,
			pVertexBuffer->getBaseVertex() + m_startIndex);

		if (isBackBeforeFront) {
			stateManager->setCullMode(CullMode::back, (flags & 2) != 0);
			stateManager->updateRenderState();
			d3d11DeviceContext->Draw(
				m_numVertices,
				pVertexBuffer->getBaseVertex() + m_startIndex);
		}
	}

	// if (stereoOverride) {
	// 	stateManager->UnsetStereoOverride();
	// 	stateManager->updateMatrices();
	// }
}

void PCDX11NGAPrimitive::SetupTransform(PCDX11NGAPrimitive *prevNGA) {
	(void) prevNGA;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	stateManager->setProjectMatrixPtr(m_pState->m_pProjectionOverrideMatrix);
	stateManager->setWorldMatrix(m_pState->m_pWorldMatrix
		? *m_pState->m_pWorldMatrix
		: identity4x4);
	stateManager->updateMatrices();
}

}
