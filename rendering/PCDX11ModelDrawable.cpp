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
		stateManager->setCullMode(D3D11_CULL_FRONT, cullMode);
		stateManager->setDepthState(D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ZERO);
	}

	ID3D11DeviceContext *d3d11DeviceContext = renderDevice->getD3DDeviceContext();
	uint32_t baseVertex = meshSub->staticVertexBuffer->getBaseVertex();
	uint32_t startIndex = tab0->startIndex + renderModel->indexBuffer->getStartIndex();
	uint32_t indexCount = triangleCount * 3;

	stateManager->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	stateManager->updateRenderState();
	d3d11DeviceContext->DrawIndexed(indexCount, startIndex, baseVertex);

	if (renderTwice) {
		stateManager->setCullMode(D3D11_CULL_NONE, cullMode);
		stateManager->updateRenderState();
		d3d11DeviceContext->DrawIndexed(indexCount, startIndex, baseVertex);
	}
}

}
