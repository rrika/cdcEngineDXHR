#include "PCDeviceManager.h"
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
	// typeID = kDrawableTypeIDModel;
	this->flags = (primGroup[0].triangleCount << 8) | (flags & 0xff);
	// TODO
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

}
