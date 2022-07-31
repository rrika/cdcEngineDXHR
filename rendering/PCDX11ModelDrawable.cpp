#include <d3d11.h>
#include "buffers/PCDX11IndexBuffer.h"
#include "buffers/PCDX11SimpleStaticIndexBuffer.h"
#include "buffers/PCDX11SimpleStaticVertexBuffer.h"
#include "PCDX11LightManager.h"
#include "PCDX11Material.h"
#include "PCDX11MatrixState.h" // for PoseData
#include "PCDX11ModelDrawable.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderModel.h"
#include "PCDX11StateManager.h"
#include "PCDX11StreamDecl.h"
#include "VertexAttribute.h"

namespace cdc {

// use of this global variable makes this class thread-unsafe
static float matrixStagingBuffer[40 * 16];

PCDX11ModelDrawable::PCDX11ModelDrawable(
	PCDX11RenderModel *renderModel,
	ModelDrawableExt *ext,
	ModelBatch *meshSub,
	PrimGroup *primGroup,
	PersistentPGData *tab0Ext128,
	PoseData *poseData)
:
	renderModel(renderModel),
	ext(ext),
	meshSub(meshSub),
	primGroup(primGroup),
	tab0Ext128(tab0Ext128),
	poseData(poseData)
{ // hack
	word4 = 1; // use RenderModel drawers
	flags34 = (primGroup[0].triangleCount << 8);

	auto lightManager = static_cast<PCDX11LightManager*>(renderModel->renderDevice->lightManager);
	lightReceiverData = lightManager->makeReceiver(/*TODO*/);
}

// for funcset 1: renderpasses 0 and 14 (depth)
void PCDX11ModelDrawable::draw1(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl015(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		mesh->vsSelect4C,
		false,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->float2C,
		thisModel->float30);

	thisModel->draw(renderDevice, stateManager, streamDecl, false);
}

// for funcset 2: renderpass 9
void PCDX11ModelDrawable::draw2(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	/*auto *lightManager = static_cast<PCDX11LightManager*>(renderDevice->lightManager84);
	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl01(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		(*(char**)lightManager->ptr434)[276],
		mesh->vsSelect4C,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->float2C,
		thisModel->float30); */

	PCDX11StreamDecl *streamDecl = nullptr; // TODO
	thisModel->draw(renderDevice, stateManager, streamDecl, false);
}

// for funcset 7: renderpass 6 (AlphaBloomFsx)
void PCDX11ModelDrawable::draw7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl4(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		mesh->vsSelect4C,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		0.0f);
	bool renderTwice = (mt0x128->material->materialBlob->dword18 >> 11) & 1;

	thisModel->draw(renderDevice, stateManager, streamDecl, renderTwice);
}

// for funcset 4: renderpass 1 (Composite)
void PCDX11ModelDrawable::draw4(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl038(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		thisModel->lightConstantBufferData,
		mesh->vsSelect4C,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		0,
		thisModel->float2C,
		thisModel->float30);

	if (!thisModel->isUnlit())
		thisModel->buildAndAssignLightBuffer(renderDevice, stateManager);

	thisModel->draw(renderDevice, stateManager, streamDecl, false);
}

// for funcset 5: renderpasses 4, 5 and 13 (Fullscreen, PostFx and DeferredShading)
// for funcset 6: renderpass 7 (Predator)
void PCDX11ModelDrawable::draw56(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl038(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		thisModel->lightConstantBufferData,
		mesh->vsSelect4C,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		1,
		thisModel->float2C,
		thisModel->float30);
	bool renderTwice = (mt0x128->material->materialBlob->dword18 >> 11) & 1;

	if (!thisModel->isUnlit())
		thisModel->buildAndAssignLightBuffer(renderDevice, stateManager);

	thisModel->draw(renderDevice, stateManager, streamDecl, renderTwice);
}

// for funcset 10: renderpass 12 (Normal)
void PCDX11ModelDrawable::drawA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl7(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		mesh->vsSelect4C,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->float2C,
		thisModel->float30);

	thisModel->draw(renderDevice, stateManager, streamDecl, false);
}

// for funcset 1: renderpasses 0 and 14 (depth)
// for funcset 2: renderpass 9
// for funcset 8: renderpass 8
bool PCDX11ModelDrawable::compare128(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}

// for funcset 7: renderpass 6 (AlphaBloomFsx)
bool PCDX11ModelDrawable::compare7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}

// for funcset 4: renderpass 1 (Composite)
// for funcset 6: renderpass 7 (Predator)
bool PCDX11ModelDrawable::compare46(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}

// for funcset 10: renderpass 12 (Normal)
bool PCDX11ModelDrawable::compareA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}


void PCDX11ModelDrawable::draw(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) {
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	PCDX11RenderDevice *renderDevice = renderModel->renderDevice;

	uint32_t shaderIndex = 0;
	PCDX11VertexShader *vertexShader = renderDevice->shtab_vs_wvp_1_0.vertexShaders[shaderIndex];
	stateManager->setVertexShader(vertexShader);

	// TODO
	// renderDevice->setTexture(0, renderDevice->missingTexture0, /*format=*/256, 0.0);
	// renderDevice->setTexture(1, renderDevice->missingTexture1, /*format=*/256, 0.0);

	stateManager->setRenderTargetWriteMask(
		(tab0Ext128->sub10.dword14 & 0x100) ? 0 :
		renderDevice->scene78->byte25C ? 7 : 15);

	auto *layout = (VertexAttributeLayoutA*)meshSub->format;
	PCDX11StreamDecl *streamDecl = renderDevice->streamDeclCache.buildStreamDecl(layout, &vertexShader->m_sub);

	draw(renderDevice, stateManager, streamDecl, false);
}

uint32_t PCDX11ModelDrawable::compare(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) {
	// TODO
	return 0;
}

void PCDX11ModelDrawable::draw(
	PCDX11RenderDevice *renderDevice,
	PCDX11StateManager *stateManager,
	PCDX11StreamDecl *streamDecl,
	bool renderTwice)
{
	if (streamDecl == nullptr)
		return;

	Mesh *mesh = renderModel->getMesh();
	stateManager->setDepthLayer(mesh->flags.depthLayer);
	stateManager->setIndexBuffer(renderModel->indexBuffer);
	stateManager->setVertexBuffer(meshSub->staticVertexBuffer);
	stateManager->setStreamDecl(streamDecl);

	if (renderTwice) {
		stateManager->setCullMode(CullMode::front, getCullMode());
		stateManager->setDepthState(D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ZERO);
	}

	ID3D11DeviceContext *d3d11DeviceContext = renderDevice->getD3DDeviceContext();
	uint32_t baseVertex = meshSub->staticVertexBuffer->getBaseVertex();
	uint32_t startIndex = primGroup->startIndex + renderModel->indexBuffer->getStartIndex();
	uint32_t indexCount = getTriangleCount() * 3;

	stateManager->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	stateManager->updateRenderState();
	d3d11DeviceContext->DrawIndexed(indexCount, startIndex, baseVertex);

	if (renderTwice) {
		stateManager->setCullMode(CullMode::back, getCullMode());
		stateManager->updateRenderState();
		d3d11DeviceContext->DrawIndexed(indexCount, startIndex, baseVertex);
	}
}

bool PCDX11ModelDrawable::setMatrices(
	PCDX11StateManager *stateManager,
	PCDX11ModelDrawable *prevDrawable,
	bool hasBones)
{
	ModelBatch *prevModelBatch = prevDrawable ? prevDrawable->meshSub : nullptr;
	PoseData *prevPoseData = prevDrawable ? prevDrawable->poseData : nullptr;
	if (hasBones) {
		if (meshSub != prevModelBatch || poseData != prevPoseData) {
			for (uint32_t i = 0; i < meshSub->commonCb3_numMatrices; i++) {
				uint32_t j = meshSub->matrixGatherOffsets[i];
				float *matrix = poseData->getMatrix(j);
				float *vector = poseData->getVector(j);

				// transpose
				matrixStagingBuffer[0] = matrix[0];
				matrixStagingBuffer[1] = matrix[4];
				matrixStagingBuffer[2] = matrix[8];
				matrixStagingBuffer[3] = matrix[12];

				matrixStagingBuffer[4] = matrix[1];
				matrixStagingBuffer[5] = matrix[5];
				matrixStagingBuffer[6] = matrix[9];
				matrixStagingBuffer[7] = matrix[13];

				matrixStagingBuffer[8] = matrix[2];
				matrixStagingBuffer[9] = matrix[6];
				matrixStagingBuffer[10] = matrix[10];
				matrixStagingBuffer[11] = matrix[14];

				// last row is different
				matrixStagingBuffer[12] = vector[0];
				matrixStagingBuffer[13] = vector[1];
				matrixStagingBuffer[14] = vector[2];
				matrixStagingBuffer[15] = vector[3];
			}
			auto &skinningBuffer = stateManager->accessCommonCB(3);
			skinningBuffer.assignRow(0, matrixStagingBuffer, 4 * meshSub->commonCb3_numMatrices);
		}
	}

	// ModelDrawableExt *currentExt = ext && ext->projectMatrixValid ? ext : nullptr;
	// ModelDrawableExt *prevExt = prevDrawable && prevDrawable->ext && prevDrawable->ext->projectMatrixValid
	// 	? prevDrawable->ext : nullptr;

	if (poseData != prevPoseData) { // || currentExt != prevExt) {
		// stateManager->setProjectMatrix(currentExt->projectMatrix);
		// TODO: don't reinterpret cast
		stateManager->setWorldMatrix(*reinterpret_cast<float4x4*>(poseData->getMatrix(0)));
		stateManager->updateMatrices();
		return poseData != prevPoseData;
	}

	return false;
}

void PCDX11ModelDrawable::buildAndAssignLightBuffer(
	PCDX11RenderDevice *renderDevice,
	PCDX11StateManager *stateManager)
{
	auto lightManager = static_cast<PCDX11LightManager*>(renderModel->renderDevice->lightManager);
    // if (lightManager->lightDataX_E10 != lightReceiverData)
        lightManager->fillLightBuffer(lightReceiverData);

    if (lightConstantBufferData) {
        // if (lightManager->mostRecentAssignmentToCommonCB5 != lightConstantBufferData)
            lightManager->assignCommonCB5((char*)lightConstantBufferData);
    }
}

}
