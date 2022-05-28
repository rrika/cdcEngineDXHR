#include "PCDX11Material.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11StateManager.h"
#include "PCDX11Texture.h"
#include "PCDX11UberConstantBuffer.h"

namespace cdc {

void PCDX11Material::load(MaterialBlob *) {
	// TODO
}

void PCDX11Material::method_08() {
	// TODO
}

void PCDX11Material::method_18() {
	// TODO
}

void PCDX11Material::setupVertexResources(
	uint32_t cb4Index,
	MaterialBlobSub* subMat,
	MeshTab0Ext128Sub10* subExt,
	char *cbData,
	bool doEverything)
{
	auto *stateManager = deviceManager->getStateManager();
	auto *texref = subMat->vsTextureRef;

	if (doEverything) {

		if (subMat->vsBufferSize)
			stateManager->setVsConstantBuffer(4, constantBuffersVs[cb4Index]);

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
	uint32_t cb3Index,
	MaterialBlobSub* subMat,
	MeshTab0Ext128Sub10* subExt,
	char *cbData,
	bool doEverything)
{
	auto *stateManager = deviceManager->getStateManager();
	auto *texref = subMat->psTextureRef;

	if (!doEverything) {

		if (subMat->psBufferSize)
			stateManager->setPsConstantBuffer(3, constantBuffersPs[cb3Index]);

		// assign 0..refIndexEndA from submaterial
		for (uint32_t i = 0; i < subMat->psRefIndexEndA; i++)
			renderDevice->setTexture(
				texref[i].slotIndex,
				static_cast<PCDX11Texture*>(texref[i].tex),
				texref[i].filter,
				texref[i].unknown4);

		// assign refIndexEndA..refIndexBeginB from submaterial or CommonSceneSub114
		CommonScene *scene = renderDevice->scene78;
		for (uint32_t i = subMat->psRefIndexEndA; i < subMat->psRefIndexBeginB; i++) {
			auto fi = texref[i].fallbackIndex & 0x1F;
			PCDX11Texture* tex = static_cast<PCDX11Texture*>(scene->sub114.tex14[fi]);
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
		PCDX11Texture* tex = extTextures[fi - 1];
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
	MeshTab0Ext128Sub10*,
	void *drawableExtDword50,
	uint32_t vsSelect,
	bool arg4,
	VertexAttributeLayoutA *layout,
	uint8_t flags,
	float floatX,
	float floatY)
{
	// TODO
	return nullptr;
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
	uint32_t drawableDword24,
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
	MeshTab0Ext128Sub10*,
	void *drawableExtDword50,
	uint32_t vsSelect,
	VertexAttributeLayoutA *layout,
	uint8_t flags,
	float floatX,
	float floatY)
{
	// TODO
	return nullptr;
}

}
