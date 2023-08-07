#include "PCDeviceManager.h"
#include "PCMaterial.h"
#include "PCModelDrawable.h"
#include "PCRenderModel.h"
#include "PCStateManager.h"
#include "buffers/PCVertexBuffer.h"
#include "buffers/PCStaticIndexBuffer.h"
#include "rendering/buffers/CommonVertexBuffer.h"
#include "rendering/Types.h"

namespace cdc {

PCModelDrawable::PCModelDrawable(
	PCRenderModel *renderModel,
	RenderModelInstanceData *ext,
	ModelBatch *modelBatch,
	PrimGroup *primGroup,
	PersistentPGData *persistentPG,
	PCPoseData *poseData,
	uint32_t numPrimGroups,
	float lodOpacity,
	uint32_t flags)
:
	renderModel(renderModel),
	ext(ext),
	modelBatch(modelBatch),
	primGroup(primGroup),
	persistentPG(persistentPG),
	poseData(poseData),
	lodOpacity(lodOpacity)
{
	typeID = kDrawableTypeIDModel;
	this->flags = (primGroup[0].triangleCount << 8) | (flags & 0xff);
	// TODO
}

void PCModelDrawable::drawNormal(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	// aka. cdc::DrawableRenderNormal
	auto *thisModel = static_cast<PCModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCModelDrawable*>(prevDrawable);
	PCRenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCStateManager *stateManager = deviceManager9->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->SetupMatrixState(stateManager, prevModel, mesh->flags.hasBones);

	PersistentPGData *persistentPG = thisModel->persistentPG;
	PCStreamDecl *streamDecl = static_cast<PCMaterial*>(persistentPG->material)->SetupNormalMapPass(
		persistentPG->sub10,
		(void*)thisModel->ext->instanceParams,
		mesh->vsSelect4C,
		(VertexDecl*)thisModel->modelBatch->format,
		(uint8_t)thisModel->flags,
		thisModel->lodOpacity);

	thisModel->DrawPrimitive(renderDevice, stateManager, streamDecl, false);
}

void PCModelDrawable::draw(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) {
	// TODO
}

uint32_t PCModelDrawable::compare(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) {
	// TODO
	return 0;
}

void PCModelDrawable::DrawPrimitive(
	PCRenderDevice *renderDevice,
	PCStateManager *stateManager,
	PCStreamDecl *streamDecl,
	bool renderTwice)
{
	if (!streamDecl)
		return;
	
	PCDeviceManager *deviceManager = deviceManager9;
	// bool skySphereMode = 
	// ...

	auto *indexBuffer = renderModel->indexBuffer;
	auto *vertexBuffer = static_cast<cdc::PCVertexBuffer*>(modelBatch->staticVertexBuffer);
	stateManager->setIndexBuffer(indexBuffer);
	stateManager->setVertexBuffer(vertexBuffer);
	stateManager->setStreamDecl(streamDecl);

	if (true) { // TODO: status is OK
		if (renderTwice) {
			// stateManager->SetCullMode(2, (flags & 2) != 0);
			// stateManager->SetDepthState(2, 0);
		}

		IDirect3DDevice9 *d3dDevice = deviceManager->getD3DDevice();
		uint32_t numPrimitives = flags >> 8;
		uint32_t startIndex = primGroup->startIndex + indexBuffer->getStartIndex();
		uint32_t numVertices = modelBatch->numVertices;
		uint32_t baseVertex = vertexBuffer->GetBaseVertexIndex();

		HRESULT hr = d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, baseVertex, 0, numVertices, startIndex, numPrimitives);

		if (renderTwice || hr < 0) {
			// stateManager->SetCullMode(1, (flags & 2) != 0);
			if (hr < 0) {
				// stateManager->SetDepthState(4, 0);
				// renderDevice->SetupErrorShaders
			}
			d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, baseVertex, 0, numVertices, startIndex, numPrimitives);
		}
	}
}

bool PCModelDrawable::SetupMatrixState(
	PCStateManager *stateManager,
	PCModelDrawable *prevDrawable,
	bool hasBones)
{
	// TODO
	return false;
}

}
