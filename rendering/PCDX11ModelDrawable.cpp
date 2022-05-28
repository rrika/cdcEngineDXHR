#include <d3d11.h>
#include "buffers/PCDX11IndexBuffer.h"
#include "buffers/PCDX11SimpleStaticIndexBuffer.h"
#include "buffers/PCDX11SimpleStaticVertexBuffer.h"
#include "PCDX11LightManager.h"
#include "PCDX11Material.h"
#include "PCDX11ModelDrawable.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderModel.h"
#include "PCDX11StateManager.h"
#include "PCDX11StreamDecl.h"
#include "VertexAttribute.h"

namespace cdc {

PCDX11ModelDrawable::PCDX11ModelDrawable(
	PCDX11RenderModel *renderModel,
	MeshSub *meshSub,
	MeshTab0 *tab0,
	MeshTab0Ext128 *tab0Ext128)
:
	renderModel(renderModel),
	meshSub(meshSub),
	tab0(tab0),
	tab0Ext128(tab0Ext128)
{ // hack
	word4 = 1; // use RenderModel drawers
	flags34 = (tab0[0].triangleCount << 8);

	auto lightManager = static_cast<PCDX11LightManager*>(renderModel->renderDevice->lightManager);
	lightReceiverData = lightManager->makeReceiver(/*TODO*/);
}

// for funcset 1: renderpasses 0 and 14 (depth)
bool PCDX11ModelDrawable::draw1(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	MeshTab0Ext128 *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl015(
		&mt0x128->sub10,
		&thisModel->ext->dword50,
		mesh->vsSelect4C,
		false,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->float2C,
		thisModel->float30);

	thisModel->draw(renderDevice, stateManager, streamDecl, false);
	return false;
}

// for funcset 2: renderpass 9
bool PCDX11ModelDrawable::draw2(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	/*auto *lightManager = static_cast<PCDX11LightManager*>(renderDevice->lightManager84);
	MeshTab0Ext128 *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl01(
		&mt0x128->sub10,
		&thisModel->ext->dword50,
		(*(char**)lightManager->ptr434)[276],
		mesh->vsSelect4C,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->float2C,
		thisModel->float30); */

	PCDX11StreamDecl *streamDecl = nullptr; // TODO
	thisModel->draw(renderDevice, stateManager, streamDecl, false);
	return false;
}

// for funcset 7: renderpass 6 (AlphaBloomFsx)
bool PCDX11ModelDrawable::draw7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	MeshTab0Ext128 *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl4(
		&mt0x128->sub10,
		&thisModel->ext->dword50,
		mesh->vsSelect4C,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		0.0f);
	bool renderTwice = (mt0x128->material->materialBlob->dword18 >> 11) & 1;

	thisModel->draw(renderDevice, stateManager, streamDecl, renderTwice);
	return false;
}

// for funcset 4: renderpass 1 (Composite)
bool PCDX11ModelDrawable::draw4(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	MeshTab0Ext128 *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl038(
		&mt0x128->sub10,
		&thisModel->ext->dword50,
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
	return false;
}

// for funcset 5: renderpasses 4, 5 and 13 (Fullscreen, PostFx and DeferredShading)
// for funcset 6: renderpass 7 (Predator)
bool PCDX11ModelDrawable::draw56(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	MeshTab0Ext128 *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl038(
		&mt0x128->sub10,
		&thisModel->ext->dword50,
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
	return false;
}

// for funcset 10: renderpass 12 (Normal)
bool PCDX11ModelDrawable::drawA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	PCDX11RenderDevice *renderDevice = thisModel->renderModel->renderDevice;
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	Mesh *mesh = thisModel->renderModel->mesh;

	thisModel->setMatrices(stateManager, prevModel, mesh->flags.hasBones);

	MeshTab0Ext128 *mt0x128 = thisModel->tab0Ext128;
	PCDX11StreamDecl *streamDecl = mt0x128->material->buildStreamDecl7(
		&mt0x128->sub10,
		&thisModel->ext->dword50,
		mesh->vsSelect4C,
		(VertexAttributeLayoutA*)thisModel->meshSub->format,
		(uint8_t)thisModel->flags34,
		thisModel->float2C,
		thisModel->float30);

	thisModel->draw(renderDevice, stateManager, streamDecl, false);
	return false;
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
		stateManager->setCullMode(D3D11_CULL_FRONT, getCullMode());
		stateManager->setDepthState(D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ZERO);
	}

	ID3D11DeviceContext *d3d11DeviceContext = renderDevice->getD3DDeviceContext();
	uint32_t baseVertex = meshSub->staticVertexBuffer->getBaseVertex();
	uint32_t startIndex = tab0->startIndex + renderModel->indexBuffer->getStartIndex();
	uint32_t indexCount = getTriangleCount() * 3;

	stateManager->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	stateManager->updateRenderState();
	d3d11DeviceContext->DrawIndexed(indexCount, startIndex, baseVertex);

	if (renderTwice) {
		stateManager->setCullMode(D3D11_CULL_NONE, getCullMode());
		stateManager->updateRenderState();
		d3d11DeviceContext->DrawIndexed(indexCount, startIndex, baseVertex);
	}
}

void PCDX11ModelDrawable::setMatrices(
	PCDX11StateManager *stateManager,
	PCDX11ModelDrawable *prevDrawable,
	bool hasBones)
{
	// TODO
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
