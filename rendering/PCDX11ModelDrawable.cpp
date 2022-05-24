#include <d3d11.h>
#include "PCDX11IndexBuffer.h"
#include "PCDX11ModelDrawable.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderModel.h"
#include "PCDX11SimpleStaticIndexBuffer.h"
#include "PCDX11SimpleStaticVertexBuffer.h"
#include "PCDX11StateManager.h"
#include "PCDX11StreamDecl.h"
#include "VertexAttribute.h"

namespace cdc {

// for funcset 1: renderpasses 0 and 14 (depth)
bool PCDX11ModelDrawable::draw1(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}

// for funcset 2: renderpass 9
bool PCDX11ModelDrawable::draw2(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}

// for funcset 7: renderpass 6 (AlphaBloomFsx)
bool PCDX11ModelDrawable::draw7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}

// for funcset 4: renderpass 1 (Composite)
bool PCDX11ModelDrawable::draw4(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}

// for funcset 5: renderpasses 4, 5 and 13 (Fullscreen, PostFx and DeferredShading)
// for funcset 6: renderpass 7 (Predator)
bool PCDX11ModelDrawable::draw56(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
	return false;
}

// for funcset 10: renderpass 12 (Normal)
bool PCDX11ModelDrawable::drawA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable) {
	auto *thisModel = static_cast<PCDX11ModelDrawable*>(drawable);
	auto *prevModel = static_cast<PCDX11ModelDrawable*>(prevDrawable);
	// TODO
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
	auto *elementDesc = new D3D11_INPUT_ELEMENT_DESC[layout->numAttr];
	// printf("decodeVertexAttribA count = %d\n", layout->numAttr);
	memset(elementDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC[layout->numAttr]));
	decodeVertexAttribA(elementDesc, layout->attrib, layout->numAttr, vertexShader->wineWorkaround);

	PCDX11StreamDecl streamDecl(renderDevice, elementDesc, layout->numAttr, &vertexShader->m_sub);
	draw(renderDevice, stateManager, &streamDecl, false);
	delete[] elementDesc;
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

}
