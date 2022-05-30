#include "PCDX11DeviceManager.h"
#include "PCDX11Material.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11StateManager.h"
#include "RenderMesh.h"
#include "surfaces/PCDX11Texture.h"
#include "buffers/PCDX11UberConstantBuffer.h"

namespace cdc {

void PCDX11Material::load(MaterialBlob *newBlob) {
	auto deviceContext = renderDevice->getD3DDeviceContext(); // HACK
	// TODO
	materialBlob = newBlob;
	// TODO
	for (uint32_t i = 0; i < 16; i++) {
		MaterialBlobSub *sub = materialBlob->subMat4C[i];

		if (!sub) continue;
		if (!sub->shaderPixel && !sub->shaderVertex) continue;

		// TODO: textures
		if (sub->psBufferSize) {
			auto buffer = new PCDX11UberConstantBuffer(32);
			buffer->assignRow(0, sub->psBufferData, sub->psBufferSize);
			buffer->syncBuffer(deviceContext); // HACK
			constantBuffersPs[i] = buffer;
		}

		if (sub->vsBufferSize) {
			auto buffer = new PCDX11UberConstantBuffer(32);
			buffer->assignRow(0, sub->vsBufferData, sub->vsBufferSize);
			buffer->syncBuffer(deviceContext); // HACK
			constantBuffersVs[i] = buffer;
		}
	}
}

void PCDX11Material::method_08() {
	// TODO
}

void PCDX11Material::method_18() {
	// TODO
}

void PCDX11Material::setupVertexResources(
	uint32_t subMaterialIndex,
	MaterialBlobSub* subMat,
	MeshTab0Ext128Sub10* subExt,
	char *cbData,
	bool doEverything)
{
	auto *stateManager = deviceManager->getStateManager();
	auto *texref = subMat->vsTextureRef;

	if (doEverything) {

		if (subMat->vsBufferSize)
			stateManager->setVsConstantBuffer(4, constantBuffersVs[subMaterialIndex]);

		// assign 0..refIndexEndA from submaterial
		for (uint32_t i = 0; i < subMat->vsRefIndexEndA; i++)
			renderDevice->setTexture(
				texref[i].slotIndex + 257,
				static_cast<PCDX11Texture*>(texref[i].tex),
				texref[i].filter,
				texref[i].unknown4);

		// assign refIndexEndA..refIndexBeginB from submaterial or CommonSceneSub114
		CommonScene *scene = renderDevice->scene78;
		for (uint32_t i = subMat->vsRefIndexEndA; i < subMat->vsRefIndexBeginB; i++) {
			auto fi = texref[i].fallbackIndex & 0x1F;
			PCDX11Texture* tex = static_cast<PCDX11Texture*>(scene->sub114.tex14[fi]);
			if (!tex) tex = static_cast<PCDX11Texture*>(texref[i].tex);
			renderDevice->setTexture(
				texref[i].slotIndex + 257,
				tex,
				fi == 10 ? 0 : texref[i].filter,
				texref[i].unknown4);
		}
	}

	// assign refIndexBeginB..refIndexEndB from submaterial or MeshTab0Ext128Sub10
	for (uint32_t i = subMat->vsRefIndexBeginB; i < subMat->vsRefIndexEndB; i++) {
		auto extTextures = (PCDX11Texture**)subExt;
		auto fi = texref[i].fallbackIndex & 0x1F;
		PCDX11Texture* tex = extTextures[fi - 1];
		if (!tex) tex = static_cast<PCDX11Texture*>(texref[i].tex);
		renderDevice->setTexture(
			texref[i].slotIndex + 257,
			tex,
			texref[i].filter,
			texref[i].unknown4);
	}

	if (subMat->vsBufferNumRows) {
		PCDX11UberConstantBuffer& cb = stateManager->accessCommonCB(4);
		auto firstRow = subMat->vsBufferFirstRow;
		auto numRows = subMat->vsBufferNumRows;
		cb.assignRow(firstRow, &cbData[16 * firstRow], numRows);
	}
}

void PCDX11Material::setupPixelResources(
	uint32_t subMaterialIndex,
	MaterialBlobSub* subMat,
	MeshTab0Ext128Sub10* subExt,
	char *cbData,
	bool doEverything)
{
	auto *stateManager = deviceManager->getStateManager();
	auto *texref = subMat->psTextureRef;

	if (doEverything) {

		if (subMat->psBufferSize)
			stateManager->setPsConstantBuffer(3, constantBuffersPs[subMaterialIndex]);

		// assign 0..refIndexEndA from submaterial
		for (uint32_t i = 0; i < subMat->psRefIndexEndA; i++) {
			PCDX11Texture* tex = static_cast<PCDX11Texture*>(texref[i].tex);
			renderDevice->setTexture(
				texref[i].slotIndex,
				tex,
				texref[i].filter,
				texref[i].unknown4);
		}

		// assign refIndexEndA..refIndexBeginB from submaterial or CommonSceneSub114
		CommonScene *scene = renderDevice->scene78;
		for (uint32_t i = subMat->psRefIndexEndA; i < subMat->psRefIndexBeginB; i++) {
			auto fi = texref[i].fallbackIndex & 0x1F;
			// PCDX11Texture* tex = static_cast<PCDX11Texture*>(scene->sub114.tex14[fi]);
			PCDX11Texture* tex = nullptr; // HACK
			if (!tex) tex = static_cast<PCDX11Texture*>(texref[i].tex);
			renderDevice->setTexture(
				texref[i].slotIndex,
				tex,
				fi == 10 ? 0 : texref[i].filter,
				texref[i].unknown4);
		}
	}

	// assign refIndexBeginB..refIndexEndB from submaterial or MeshTab0Ext128Sub10
	for (uint32_t i = subMat->psRefIndexBeginB; i < subMat->psRefIndexEndB; i++) {
		auto extTextures = (PCDX11Texture**)subExt;
		auto fi = texref[i].fallbackIndex & 0x1F;
		// PCDX11Texture* tex = extTextures[fi - 1];
		PCDX11Texture* tex = nullptr; // HACK
		if (!tex) tex = static_cast<PCDX11Texture*>(texref[i].tex);
		renderDevice->setTexture(
			texref[i].slotIndex,
			tex,
			texref[i].filter,
			texref[i].unknown4);
	}

	if (subMat->psBufferNumRows) {
		PCDX11UberConstantBuffer& cb = stateManager->accessCommonCB(4);
		auto firstRow = subMat->psBufferFirstRow;
		auto numRows = subMat->psBufferNumRows;
		cb.assignRow(firstRow, &cbData[16 * firstRow], numRows);
	}
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl015(
	MeshTab0Ext128Sub10* ext128sub10,
	void *drawableExtDword50,
	uint32_t vsSelect,
	bool arg4,
	VertexAttributeLayoutA *layoutA,
	uint8_t flags,
	float floatX, // opacity scale?
	float floatY)
{
	// TODO

	float opacity = ext128sub10->float10 * floatX;
	uint32_t subMaterialIndex = 0;
	if (arg4)
		subMaterialIndex = 5;

	// TODO

	MaterialBlobSub *subMaterial = materialBlob->subMat4C[subMaterialIndex];
	bool doEverything = true; // TODO
	auto *stateManager = deviceManager->getStateManager();
	uint32_t vertexIndex = vsSelect;
	if (flags & 8)
		vertexIndex |= 8;

	// set pixel shader
	PCDX11PixelShaderTable *pixelTable;
	uint32_t pixelIndex = 0;
	if (materialBlob->blendState == 0x7010010) {
		pixelTable = &renderDevice->shtab_ps_white_27;
		if (arg4)
			pixelIndex = 1;
	} else {
		pixelTable = static_cast<PCDX11PixelShaderTable*>(static_cast<PCDX11ShaderLib*>(subMaterial->shaderPixel)->table);
	}

	auto pixelShader = (*pixelTable)[pixelIndex];
	stateManager->setPixelShader(pixelShader);

	// set vertex shader
	auto vertexTable = static_cast<PCDX11VertexShaderTable*>(static_cast<PCDX11ShaderLib*>(subMaterial->shaderVertex)->table);
	auto vertexShader = (*vertexTable)[vertexIndex];
	stateManager->setVertexShader(vertexShader);

	VertexAttributeLayoutB *layoutB = nullptr; // TODO

	PCDX11StreamDecl *streamDecl = renderDevice->streamDeclCache.buildStreamDecl(
		layoutA,
		layoutB,
		(flags >> 3) & 1,
		&vertexShader->m_sub);

	// TODO
	if (true)
		setupPixelResources(subMaterialIndex, subMaterial, ext128sub10, (char*)drawableExtDword50, doEverything);
	setupVertexResources(subMaterialIndex, subMaterial, ext128sub10, (char*)drawableExtDword50, doEverything);

	// TODO
	stateManager->setOpacity(opacity);

	return streamDecl;
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl01(
	MeshTab0Ext128Sub10*,
	void *drawableExtDword50,
	uint8_t lightManager434_114,
	uint32_t vsSelect,
	VertexAttributeLayoutA *layout,
	uint8_t flags,
	float floatX,
	float floatY)
{
	// TODO
	return nullptr;
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl4(
	MeshTab0Ext128Sub10*,
	void *drawableExtDword50,
	uint32_t vsSelect,
	VertexAttributeLayoutA *layout,
	uint8_t flags,
	float floatX)
{
	// TODO
	return nullptr;
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl038(
	MeshTab0Ext128Sub10*,
	void *drawableExtDword50,
	void *lightConstantBufferData,
	uint32_t vsSelect,
	VertexAttributeLayoutA *layout,
	uint8_t flags,
	bool flag,
	float floatX,
	float floatY)
{
	// TODO
	return nullptr;
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl7(
	MeshTab0Ext128Sub10 *ext128sub10,
	void *drawableExtDword50,
	uint32_t vsSelect,
	VertexAttributeLayoutA *layout,
	uint8_t flags,
	float floatX,
	float floatY)
{
	// TODO
	// return nullptr;
	return buildStreamDecl015(
		ext128sub10,
		drawableExtDword50,
		vsSelect,
		0,
		layout,
		flags,
		floatX,
		floatY);
}

}
