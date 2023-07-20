#include <d3d11.h>
#include "buffers/PCDX11IndexBuffer.h"
#include "buffers/PCDX11StaticIndexBuffer.h"
#include "buffers/PCDX11StaticVertexBuffer.h"
#include "PCDX11LightManager.h"
#include "PCDX11Material.h"
#include "PCDX11MatrixState.h" // for PoseData
#include "PCDX11ModelDrawable.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderModel.h"
#include "PCDX11StateManager.h"
#include "PCDX11StreamDecl.h"
#include "Types.h"
#include "VertexDeclaration.h"

namespace cdc {

// use of this global variable makes this class thread-unsafe
static float matrixStagingBuffer[42 * 16];

PCDX11ModelDrawable::PCDX11ModelDrawable(
	PCDX11RenderModel *renderModel,
	RenderModelInstanceData *ext,
	ModelBatch *meshSub,
	PrimGroup *primGroup,
	PersistentPGData *tab0Ext128,
	PoseData *poseData,
	float opacity,
	uint8_t flags)
:
	renderModel(renderModel),
	ext(ext),
	meshSub(meshSub),
	primGroup(primGroup),
	tab0Ext128(tab0Ext128),
	poseData(poseData),
	opacity(opacity)
{ // hack
	typeID = kDrawableTypeIDModel;
	flags34 = (primGroup[0].triangleCount << 8) | flags;

	auto lightManager = static_cast<PCDX11LightManager*>(renderModel->renderDevice->lightManager);
	lightReceiverData = lightManager->makeReceiver(/*TODO*/);
}

// for funcset 1: renderpasses 0 and 14 (depth)
void PCDX11ModelDrawable::drawDepth(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = static_cast<PCDX11Material*>(mt0x128->material)->SetupDepthPass(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		mesh->vsSelect4C,
		false,
		(VertexDecl*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->opacity,
		thisModel->float30);

	thisModel->draw(renderDevice, stateManager, streamDecl, false);
}

// for funcset 2: renderpass 9
void PCDX11ModelDrawable::drawShadow(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	/*auto *lightManager = static_cast<PCDX11LightManager*>(renderDevice->lightManager84);
	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = static_cast<PCDX11Material*>(mt0x128->material)->SetupShadowPass(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		(*(char**)lightManager->ptr434)[276],
		mesh->vsSelect4C,
		(VertexDecl*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->opacity,
		thisModel->float30); */

	PCDX11StreamDecl *streamDecl = nullptr; // TODO
	thisModel->draw(renderDevice, stateManager, streamDecl, false);
}

// for funcset 7: renderpass 6 (AlphaBloomFsx)
void PCDX11ModelDrawable::drawAlphaBloom(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = static_cast<PCDX11Material*>(mt0x128->material)->SetupBloomPass(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		mesh->vsSelect4C,
		(VertexDecl*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		0.0f);
	bool renderTwice = (mt0x128->material->materialBlob->dword18 >> 11) & 1;

	thisModel->draw(renderDevice, stateManager, streamDecl, renderTwice);
}

// for funcset 4: renderpass 1 (Composite)
void PCDX11ModelDrawable::drawComposite(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = static_cast<PCDX11Material*>(mt0x128->material)->SetupSinglePass(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		thisModel->lightConstantBufferData,
		mesh->vsSelect4C,
		(VertexDecl*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		0,
		thisModel->opacity,
		thisModel->float30);

	if (!thisModel->isUnlit())
		thisModel->buildAndAssignLightBuffer(renderDevice, stateManager);

	thisModel->draw(renderDevice, stateManager, streamDecl, false);
}

// for funcset 5: renderpasses 4, 5 and 13 (Fullscreen, PostFx and DeferredShading)
// for funcset 6: renderpass 7 (Predator)
void PCDX11ModelDrawable::drawTranslucent(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = static_cast<PCDX11Material*>(mt0x128->material)->SetupSinglePass(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		thisModel->lightConstantBufferData,
		mesh->vsSelect4C,
		(VertexDecl*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		1,
		thisModel->opacity,
		thisModel->float30);
	bool renderTwice = (mt0x128->material->materialBlob->dword18 >> 11) & 1;

	if (!thisModel->isUnlit())
		thisModel->buildAndAssignLightBuffer(renderDevice, stateManager);

	thisModel->draw(renderDevice, stateManager, streamDecl, renderTwice);
}

// for funcset 10: renderpass 12 (Normal)
void PCDX11ModelDrawable::drawNormal(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = static_cast<PCDX11Material*>(mt0x128->material)->SetupNormalMapPass(
		&mt0x128->sub10,
		(void*)thisModel->ext->instanceParams,
		mesh->vsSelect4C,
		(VertexDecl*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->opacity,
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
bool PCDX11ModelDrawable::compareNormal(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
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
		(tab0Ext128->sub10.polyFlags & 0x100) ? 0 :
		renderDevice->scene78->byte25C ? 7 : 15);

	auto *layout = (VertexDecl*)meshSub->format;
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
	stateManager->setVertexBuffer(static_cast<PCDX11VertexBuffer*>(meshSub->staticVertexBuffer));
	stateManager->setStreamDecl(streamDecl);

	if (renderTwice) {
		stateManager->setCullMode(CullMode::front, getFrontCounterClockwise());
		stateManager->setDepthState(D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ZERO);
	}

	ID3D11DeviceContext *d3d11DeviceContext = renderDevice->getD3DDeviceContext();
	uint32_t baseVertex = static_cast<PCDX11VertexBuffer*>(meshSub->staticVertexBuffer)->getBaseVertex();
	uint32_t startIndex = primGroup->startIndex + renderModel->indexBuffer->getStartIndex();
	uint32_t indexCount = getTriangleCount() * 3;

	stateManager->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	stateManager->updateRenderState();
	d3d11DeviceContext->DrawIndexed(indexCount, startIndex, baseVertex);

	if (renderTwice) {
		stateManager->setCullMode(CullMode::back, getFrontCounterClockwise());
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
				// uint32_t j = meshSub->matrixGatherOffsets[i];
				uint32_t j = 0; // HACK
				float *matrix = poseData->getMatrix(j);
				float *vector = poseData->getVector(j);

				if (i >= 42) {
					printf("ERROR: skinningBuffer overflow\n");
					break;
				}
				float *m = &matrixStagingBuffer[16 * i];

				// transpose
				m[0] = matrix[0];
				m[1] = matrix[4];
				m[2] = matrix[8];
				m[3] = matrix[12]; // translation x (unless, I made a mistake)

				m[4] = matrix[1];
				m[5] = matrix[5];
				m[6] = matrix[9];
				m[7] = matrix[13]; // translation y

				m[8] = matrix[2];
				m[9] = matrix[6];
				m[10] = matrix[10];
				m[11] = matrix[14]; // translation z

				// last row is different
				m[12] = vector[0];
				m[13] = vector[1];
				m[14] = vector[2];
				m[15] = vector[3];
			}
			auto &skinningBuffer = stateManager->accessCommonCB(3);
			skinningBuffer.assignRow(0, matrixStagingBuffer, 4 * meshSub->commonCb3_numMatrices);
		}
	}

	// RenderModelInstanceData *currentExt = ext && ext->projectMatrixValid ? ext : nullptr;
	// RenderModelInstanceData *prevExt = prevDrawable && prevDrawable->ext && prevDrawable->ext->projectMatrixValid
	// 	? prevDrawable->ext : nullptr;

	if (poseData != prevPoseData) { // || currentExt != prevExt) {
		// stateManager->setProjectMatrix(currentExt->projectMatrix);
		// TODO: don't reinterpret cast
		stateManager->setWorldMatrix(*reinterpret_cast<Matrix*>(poseData->getMatrix(0)));
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
