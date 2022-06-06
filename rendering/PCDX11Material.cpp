#include "PCDX11DeviceManager.h"
#include "PCDX11Material.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11StateManager.h"
#include "PCDX11StreamDecl.h" // for CommonStreamDecl to PCDX11StreamDecl cast
#include "RenderMesh.h"
#include "surfaces/PCDX11Texture.h"
#include "buffers/PCDX11UberConstantBuffer.h"

namespace cdc {


uint32_t PCDX11Material::mg_state;
uint32_t PCDX11Material::mg_B37BE4;
PCDX11StreamDecl *PCDX11Material::mg_streamDecl;
PCDX11Material *PCDX11Material::mg_EAAD1C;
PCDX11Material *PCDX11Material::mg_material;
void *PCDX11Material::mg_cbdata;
MeshTab0Ext128Sub10 *PCDX11Material::mg_ext128sub10;

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

void PCDX11Material::setupDepthBias(MeshTab0Ext128Sub10 *ext128sub10) {
	float depthBias = 0.0f;
	float slopeScaledDepthBias = 0.0f;

	if (materialBlob->negDepthBias) {
		depthBias = -materialBlob->negDepthBias;
		slopeScaledDepthBias = -materialBlob->negSlopeScaledDepthBias;
	} else if (ext128sub10->dword14 & 0x20000000) {
		depthBias = -1.0f;
		slopeScaledDepthBias = -10.0f;
	}

	auto *stateManager = deviceManager->getStateManager();
	stateManager->setDepthBias(int32_t(depthBias));
	stateManager->setDepthBiasClamp(0.0f);
	stateManager->setSlopeScaledDepthBias(slopeScaledDepthBias);
}

void PCDX11Material::setupStencil(
	MeshTab0Ext128Sub10 *ext128sub10,
	bool honorRenderTwice,
	uint32_t flags)
{
	auto *stateManager = deviceManager->getStateManager();
	StencilSettings *stencilSettings = ext128sub10->stencilSettings64
		? ext128sub10->stencilSettings64
		: &materialBlob->stencilSettings34;

	uint32_t matDword18 = materialBlob->dword18;
	bool frontCounterClockwise = bool(flags & 2);
	bool stencilDoubleSided = bool(stencilSettings->back & 1);
	bool ext128DoubleSided = bool(ext128sub10->dword14 & 0x40);
	bool materialDoubleSided = bool(matDword18 & 0x80);
	bool materialRenderTwice = bool(matDword18 & 0x800);
	bool materialCullFront = bool(matDword18 & 0x2000);

	if (stencilDoubleSided || (
		(materialDoubleSided || ext128DoubleSided) &&
		!(materialRenderTwice && honorRenderTwice)
	))
		stateManager->setCullMode(CullMode::none, frontCounterClockwise);
	else if (materialCullFront)
		stateManager->setCullMode(CullMode::front, frontCounterClockwise);
	else
		stateManager->setCullMode(CullMode::back, frontCounterClockwise);

	stateManager->setStencil(stencilSettings);
}

void PCDX11Material::setupMg4(
	PCDX11RenderDevice *renderDevice,
	MeshTab0Ext128Sub10 *ext128sub10,
	uint32_t flags)
{
	auto *stateManager = deviceManager->getStateManager();
	setupStencil(ext128sub10, true, flags);
	stateManager->setDepthRange(ext128sub10->minDepth, ext128sub10->maxDepth);
	if (mg_state != 4) {
		mg_state = 21;
		mg_B37BE4 = ~0u;
		mg_material = 0;
		mg_cbdata = 0;
		mg_ext128sub10 = 0;
		mg_streamDecl = 0;
		mg_EAAD1C = 0;
		// TODO: stateManager->setBlendStateAndBlendFactors(117506064, 128, 0);
		stateManager->setOpacity(1.0);
		stateManager->setDepthState(D3D11_COMPARISON_EQUAL, 0);
		stateManager->setFogColor(renderDevice->scene78->fogColor);
		mg_state = 4;
	}

	// redo some of what setupStencil did earlier
	uint32_t matDword18 = materialBlob->dword18;
	bool frontCounterClockwise = bool(flags & 2);
	bool ext128DoubleSided = bool(ext128sub10->dword14 & 0x40);
	bool materialDoubleSided = bool(matDword18 & 0x80);
	bool materialRenderTwice = bool(matDword18 & 0x800);
	bool materialCullFront = bool(matDword18 & 0x2000);

	if ((materialDoubleSided || ext128DoubleSided) && !materialRenderTwice)
		stateManager->setCullMode(CullMode::none, frontCounterClockwise);
	else if (materialCullFront)
		stateManager->setCullMode(CullMode::front, frontCounterClockwise);
	else
		stateManager->setCullMode(CullMode::back, frontCounterClockwise);
}

void PCDX11Material::setupMg5(
	PCDX11RenderDevice *renderDevice,
	MeshTab0Ext128Sub10 *ext128sub10,
	uint32_t arg3,
	float arg4)
{

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
	uint32_t subMaterialIndex = 7; // HACK: show normals submat (7) instead of 0 or 1
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
	if (false) { // HACK to see anything other than white
	// if (materialBlob->blendState == 0x7010010) {
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


	auto *streamDecl = static_cast<PCDX11StreamDecl*>(ext128sub10->streamDecls24[subMaterialIndex]);
	if (!streamDecl) {
		VertexAttributeLayoutB *layoutB = nullptr; // TODO
		
		streamDecl = renderDevice->streamDeclCache.buildStreamDecl(
			layoutA,
			layoutB,
			(flags >> 3) & 1,
			&vertexShader->m_sub);
	}

	// TODO
	if (true)
		setupPixelResources(subMaterialIndex, subMaterial, ext128sub10, (char*)drawableExtDword50, doEverything);
	setupVertexResources(subMaterialIndex, subMaterial, ext128sub10, (char*)drawableExtDword50, doEverything);

	// TODO

	setupDepthBias(ext128sub10);
	deviceManager->getStateManager()->setDepthRange(ext128sub10->minDepth, ext128sub10->maxDepth);
	deviceManager->getStateManager()->setOpacity(opacity);
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
