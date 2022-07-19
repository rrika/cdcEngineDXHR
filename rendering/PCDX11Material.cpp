#include "PCDX11DeviceManager.h"
#include "PCDX11Material.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11StateManager.h"
#include "PCDX11StreamDecl.h" // for CommonStreamDecl to PCDX11StreamDecl cast
#include "MaterialData.h"
#include "RenderMesh.h"
#include "surfaces/PCDX11Texture.h"
#include "buffers/PCDX11UberConstantBuffer.h"

namespace cdc {


uint32_t PCDX11Material::mg_state = 0;
uint32_t PCDX11Material::mg_vsSelectAndFlags;
PCDX11StreamDecl *PCDX11Material::mg_streamDecl;
VertexAttributeLayoutA *PCDX11Material::mg_layoutA;
PCDX11Material *PCDX11Material::mg_material;
void *PCDX11Material::mg_cbdata;
MaterialInstanceData *PCDX11Material::mg_matInstance;
bool PCDX11Material::mg_tesselate;

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
	MaterialInstanceData *matInstance,
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

		// assign refIndexEndA..refIndexBeginB from submaterial or RenderGlobalState
		CommonScene *scene = renderDevice->scene78;
		for (uint32_t i = subMat->vsRefIndexEndA; i < subMat->vsRefIndexBeginB; i++) {
			auto fi = texref[i].fallbackIndex & 0x1F;
			PCDX11Texture* tex = static_cast<PCDX11Texture*>(scene->globalState.tex14[fi]);
			if (!tex) tex = static_cast<PCDX11Texture*>(texref[i].tex);
			renderDevice->setTexture(
				texref[i].slotIndex + 257,
				tex,
				fi == 10 ? 0 : texref[i].filter,
				texref[i].unknown4);
		}
	}

	// assign refIndexBeginB..refIndexEndB from submaterial or MaterialInstanceData
	for (uint32_t i = subMat->vsRefIndexBeginB; i < subMat->vsRefIndexEndB; i++) {
		auto extTextures = (PCDX11Texture**)matInstance;
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
	MaterialBlobSub *subMat,
	MaterialInstanceData *subExt,
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

		// assign refIndexEndA..refIndexBeginB from submaterial or RenderGlobalState
		CommonScene *scene = renderDevice->scene78;
		for (uint32_t i = subMat->psRefIndexEndA; i < subMat->psRefIndexBeginB; i++) {
			auto fi = texref[i].fallbackIndex & 0x1F;
			PCDX11Texture* tex = static_cast<PCDX11Texture*>(scene->globalState.tex14[fi]);
			if (!tex) tex = static_cast<PCDX11Texture*>(texref[i].tex);
			renderDevice->setTexture(
				texref[i].slotIndex,
				tex,
				fi == 10 ? 0 : texref[i].filter,
				texref[i].unknown4);
		}
	}

	// assign refIndexBeginB..refIndexEndB from submaterial or MaterialInstanceData
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

void PCDX11Material::setupDepthBias(MaterialInstanceData *matInstance) {
	float depthBias = 0.0f;
	float slopeScaledDepthBias = 0.0f;

	if (materialBlob->negDepthBias) {
		depthBias = -materialBlob->negDepthBias;
		slopeScaledDepthBias = -materialBlob->negSlopeScaledDepthBias;
	} else if (matInstance->dword14 & 0x20000000) {
		depthBias = -1.0f;
		slopeScaledDepthBias = -10.0f;
	}

	auto *stateManager = deviceManager->getStateManager();
	stateManager->setDepthBias(int32_t(depthBias));
	stateManager->setDepthBiasClamp(0.0f);
	stateManager->setSlopeScaledDepthBias(slopeScaledDepthBias);
}

void PCDX11Material::setupStencil(
	MaterialInstanceData *matInstance,
	bool honorRenderTwice,
	uint32_t flags)
{
	auto *stateManager = deviceManager->getStateManager();
	StencilSettings *stencilSettings = matInstance->stencilSettings64
		? matInstance->stencilSettings64
		: &materialBlob->stencilSettings34;

	uint32_t matDword18 = materialBlob->dword18;
	bool frontCounterClockwise = bool(flags & 2);
	bool stencilDoubleSided = bool(stencilSettings->back & 1);
	bool matInstanceDoubleSided = bool(matInstance->dword14 & 0x40);
	bool materialDoubleSided = bool(matDword18 & 0x80);
	bool materialRenderTwice = bool(matDword18 & 0x800);
	bool materialCullFront = bool(matDword18 & 0x2000);

	if (stencilDoubleSided || (
		(materialDoubleSided || matInstanceDoubleSided) &&
		!(materialRenderTwice && honorRenderTwice)
	))
		stateManager->setCullMode(CullMode::none, frontCounterClockwise);
	else if (materialCullFront)
		stateManager->setCullMode(CullMode::front, frontCounterClockwise);
	else
		stateManager->setCullMode(CullMode::back, frontCounterClockwise);

	stateManager->setStencil(stencilSettings);
}

void PCDX11Material::setupSinglePassOpaque(
	PCDX11RenderDevice *renderDevice,
	MaterialInstanceData *matInstance,
	uint32_t flags)
{
	auto *stateManager = deviceManager->getStateManager();
	setupStencil(matInstance, true, flags);
	stateManager->setDepthRange(matInstance->minDepth, matInstance->maxDepth);
	if (mg_state != 4) {
		invalidate();
		stateManager->setBlendStateAndBlendFactors(0x7010010, 128, 0);
		stateManager->setOpacity(1.0);
		stateManager->setDepthState(D3D11_COMPARISON_EQUAL, 0);
		stateManager->setFogColor(renderDevice->scene78->fogColor);
		mg_state = 4;
	}

	// redo some of what setupStencil did earlier
	uint32_t matDword18 = materialBlob->dword18;
	bool frontCounterClockwise = bool(flags & 2);
	bool matInstanceDoubleSided = bool(matInstance->dword14 & 0x40);
	bool materialDoubleSided = bool(matDword18 & 0x80);
	bool materialRenderTwice = bool(matDword18 & 0x800);
	bool materialCullFront = bool(matDword18 & 0x2000);

	if ((materialDoubleSided || matInstanceDoubleSided) && !materialRenderTwice)
		stateManager->setCullMode(CullMode::none, frontCounterClockwise);
	else if (materialCullFront)
		stateManager->setCullMode(CullMode::front, frontCounterClockwise);
	else
		stateManager->setCullMode(CullMode::back, frontCounterClockwise);
}

void PCDX11Material::setupSinglePassTranslucent(
	PCDX11RenderDevice *renderDevice,
	MaterialInstanceData *matInstance,
	uint32_t arg3,
	float floatX)
{
	// lights use this function (eg. deferred_fast_omni_diffuse.drm)
	auto *stateManager = deviceManager->getStateManager();
	if (mg_state != 5) {
		invalidate();
		mg_state = 5;
	}

	float opacity = matInstance->float10 * floatX;
	uint32_t blendState = materialBlob->blendStateC;
	if ((blendState & 1) || opacity >= 1.0) {
		stateManager->setBlendStateAndBlendFactors( // TODO: impl in StateManager
			materialBlob->blendStateC,
			materialBlob->alphaThreshold,
			materialBlob->blendFactors);
	} else {
		// TODO
	}

	stateManager->setDepthState(D3D11_COMPARISON_ALWAYS, 0);


	// redo some of what setupStencil did earlier
	/*uint32_t matDword18 = materialBlob->dword18;
	bool frontCounterClockwise = bool(flags & 2);
	bool matInstanceDoubleSided = bool(matInstance->dword14 & 0x40);
	bool materialDoubleSided = bool(matDword18 & 0x80);
	bool materialRenderTwice = bool(matDword18 & 0x800);
	bool materialCullFront = bool(matDword18 & 0x2000);

	if ((materialDoubleSided || matInstanceDoubleSided) && !materialRenderTwice)
		stateManager->setCullMode(CullMode::none, frontCounterClockwise);
	else if (materialCullFront)
		stateManager->setCullMode(CullMode::front, frontCounterClockwise);
	else
		stateManager->setCullMode(CullMode::back, frontCounterClockwise);*/
}

void PCDX11Material::invalidate() {
	mg_state = 21;
	mg_vsSelectAndFlags = 0xffffffff;
	mg_material = nullptr;
	mg_cbdata = nullptr;
	mg_matInstance = nullptr;
	mg_streamDecl = nullptr;
	mg_layoutA = nullptr;
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl015(
	MaterialInstanceData* matInstance,
	void *drawableExtDword50,
	uint32_t vsSelect,
	bool arg4,
	VertexAttributeLayoutA *layoutA,
	uint8_t flags,
	float floatX, // opacity scale?
	float floatY)
{
	if (mg_state != 1) {
		mg_vsSelectAndFlags = -1;
		mg_material = nullptr;
		mg_cbdata = nullptr;
		mg_matInstance = nullptr;
		mg_streamDecl = nullptr;
		mg_layoutA = nullptr;
		mg_state = 1;
	}

	float opacity = matInstance->float10 * floatX;
	uint32_t blendState = materialBlob->blendStateC;
	bool noPixelShader = true;
	if ((blendState & 1) || (blendState & 0x7000000) != 0x7000000 || opacity < 1.0)
		noPixelShader = false;

	uint32_t subMaterialIndex;
	if (arg4)
		subMaterialIndex = 5;
	else
		subMaterialIndex = noPixelShader ? 0 : 1;

	subMaterialIndex = 7; // HACK: show normals submat (7) instead of 0 or 1

	MaterialBlobSub *subMaterial = materialBlob->subMat4C[subMaterialIndex];
	uint32_t vsSelectAndFlags = (vsSelect << 8) | flags;
	bool materialChange = mg_material != this;
	if (materialChange || mg_vsSelectAndFlags != vsSelectAndFlags) {

		auto *stateManager = deviceManager->getStateManager();
		uint32_t vertexIndex = vsSelect;
		if (flags & 8)
			vertexIndex |= 8;

		// set pixel shader
		PCDX11PixelShaderTable *pixelTable;
		uint32_t pixelIndex = 0;
		if (false) { // HACK to see anything other than white
		// if (materialBlob->blendStateC == 0x7010010) {
			stateManager->setBlendStateAndBlendFactors(0x7010010, 128, 0);
			pixelTable = &renderDevice->shtab_ps_white_27;
			if (arg4)
				pixelIndex = 1;
		} else {
			if (arg4) {
				stateManager->setBlendStateAndBlendFactors(0x7010010, 128, 0);
				stateManager->setAlphaThreshold(materialBlob->alphaThreshold);
			} else {
				stateManager->setBlendStateAndBlendFactors(0x6010010, materialBlob->alphaThreshold, 0);
			}
			pixelTable = static_cast<PCDX11PixelShaderTable*>(static_cast<PCDX11ShaderLib*>(subMaterial->shaderPixel)->table);
		}

		auto pixelShader = (*pixelTable)[pixelIndex];
		stateManager->setPixelShader(pixelShader);

		// set vertex shader
		auto vertexTable = static_cast<PCDX11VertexShaderTable*>(static_cast<PCDX11ShaderLib*>(subMaterial->shaderVertex)->table);
		auto vertexShader = (*vertexTable)[vertexIndex];
		stateManager->setVertexShader(vertexShader);

		mg_vsSelectAndFlags = vsSelectAndFlags;
		mg_material = this;

		auto *streamDecl = static_cast<PCDX11StreamDecl*>(matInstance->streamDecls24[subMaterialIndex]);
		if (!streamDecl) {
			VertexAttributeLayoutB *layoutB = nullptr; // TODO

			streamDecl = renderDevice->streamDeclCache.buildStreamDecl(
				layoutA,
				layoutB,
				(flags >> 3) & 1,
				&vertexShader->m_sub);
		}

		mg_streamDecl = streamDecl;

	} else {
		if (mg_cbdata == drawableExtDword50 || mg_matInstance == matInstance)
			return mg_streamDecl;
	}

	if (!noPixelShader)
		setupPixelResources(subMaterialIndex, subMaterial, matInstance, (char*)drawableExtDword50, materialChange);
	setupVertexResources(subMaterialIndex, subMaterial, matInstance, (char*)drawableExtDword50, materialChange);

	setupDepthBias(matInstance);
	deviceManager->getStateManager()->setDepthRange(matInstance->minDepth, matInstance->maxDepth);
	deviceManager->getStateManager()->setOpacity(opacity);
	mg_matInstance = matInstance;
	mg_cbdata = drawableExtDword50;

	return mg_streamDecl;
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl01(
	MaterialInstanceData*,
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
	MaterialInstanceData *matInstance,
	void *drawableExtDword50,
	uint32_t vsSelect,
	VertexAttributeLayoutA *layoutA,
	uint8_t flags,
	float floatX)
{
	auto *stateManager = deviceManager->getStateManager();
	const uint32_t subMaterialIndex = 4;
	MaterialBlobSub *subMaterial = materialBlob->subMat4C[subMaterialIndex];

	setupDepthBias(matInstance);
	setupStencil(matInstance, true, flags);
	stateManager->setDepthRange(matInstance->minDepth, matInstance->maxDepth);
	stateManager->setBlendStateAndBlendFactors(materialBlob->blendState24, 0, 0);

	uint32_t vertexIndex = vsSelect;
	if (flags & 8)
		vertexIndex |= 8;
	uint32_t pixelIndex = 0;

	auto pixelTable = static_cast<PCDX11PixelShaderTable*>(static_cast<PCDX11ShaderLib*>(subMaterial->shaderPixel)->table);
	auto pixelShader = (*pixelTable)[pixelIndex];
	stateManager->setPixelShader(pixelShader);

	auto vertexTable = static_cast<PCDX11VertexShaderTable*>(static_cast<PCDX11ShaderLib*>(subMaterial->shaderVertex)->table);
	auto vertexShader = (*vertexTable)[vertexIndex];
	stateManager->setVertexShader(vertexShader);

	setupPixelResources(subMaterialIndex, subMaterial, matInstance, (char*)drawableExtDword50, true);
	setupVertexResources(subMaterialIndex, subMaterial, matInstance, (char*)drawableExtDword50, true);

	auto *streamDecl = static_cast<PCDX11StreamDecl*>(matInstance->streamDecls24[subMaterialIndex]);
	if (!streamDecl) {
		VertexAttributeLayoutB *layoutB = nullptr; // TODO

		streamDecl = renderDevice->streamDeclCache.buildStreamDecl(
			layoutA,
			layoutB,
			(flags >> 3) & 1,
			&vertexShader->m_sub);
	}

	return streamDecl;
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl038(
	MaterialInstanceData *matInstance,
	void *drawableExtDword50,
	void *lightConstantBufferData,
	uint32_t vsSelect,
	VertexAttributeLayoutA *layoutA,
	uint8_t flags,
	bool flag,
	float floatX,
	float floatY)
{
	float opacity = matInstance->float10 * floatX;
	uint32_t blendState = materialBlob->blendStateC;
	bool x = true;
	if ((materialBlob->dword18 & 1) == 0 ||
		renderDevice->scene78->byte25C ||
		(blendState & 1) ||
		opacity < 1.0
	)
		x = false;

	uint32_t subMaterialIndex;
	if (flag) {
		subMaterialIndex = 8;
		setupSinglePassTranslucent(renderDevice, /*0,*/ matInstance, flags, floatX);
	} else {
		subMaterialIndex = 3;
		setupSinglePassOpaque(renderDevice, /*0,*/ matInstance, flags);
	}

	bool noPixelShader = flags & 1;
	if (noPixelShader)
		subMaterialIndex = 0;

	auto *stateManager = deviceManager->getStateManager();
	MaterialBlobSub *subMaterial = materialBlob->subMat4C[subMaterialIndex];

	if (mg_material != this) {
		setupPixelResources(subMaterialIndex, subMaterial, matInstance, (char*)drawableExtDword50, true);
		setupVertexResources(subMaterialIndex, subMaterial, matInstance, (char*)drawableExtDword50, true);
		setupDepthBias(matInstance);

	} else if (mg_matInstance != matInstance || mg_cbdata != drawableExtDword50) {
		setupPixelResources(subMaterialIndex, subMaterial, matInstance, (char*)drawableExtDword50, false);
		setupVertexResources(subMaterialIndex, subMaterial, matInstance, (char*)drawableExtDword50, false);
		setupDepthBias(matInstance);
	}

	bool tesselate = false;
	uint32_t vsSelectAndFlags = (vsSelect << 8) | flags;

	//setupStencil(matInstance, true, flags);
	stateManager->setDepthRange(matInstance->minDepth, matInstance->maxDepth);
	stateManager->setBlendStateAndBlendFactors(materialBlob->blendState24, 0, 0);

	uint16_t renderTargetWriteMask = materialBlob->renderTargetWriteMask;
	/* if (renderDevice->scene78->byte25C) {
		if (x)
			renderTargetWriteMask = renderTargetWriteMask | 8;
		else
			renderTargetWriteMask = renderTargetWriteMask & 7;
	} */
	stateManager->setRenderTargetWriteMask(renderTargetWriteMask);

	uint32_t vertexIndex = vsSelect;
	if (flags & 8)
		vertexIndex |= 8;
	uint32_t pixelIndex = 0;

	auto pixelTable = static_cast<PCDX11PixelShaderTable*>(static_cast<PCDX11ShaderLib*>(subMaterial->shaderPixel)->table);
	auto pixelShader = (*pixelTable)[pixelIndex];
	stateManager->setPixelShader(pixelShader);

	auto vertexTable = static_cast<PCDX11VertexShaderTable*>(static_cast<PCDX11ShaderLib*>(subMaterial->shaderVertex)->table);
	auto vertexShader = (*vertexTable)[vertexIndex];
	stateManager->setVertexShader(vertexShader);

	auto *streamDecl = static_cast<PCDX11StreamDecl*>(matInstance->streamDecls24[subMaterialIndex]);
	if (!streamDecl) {
		VertexAttributeLayoutB *layoutB = subMaterial->vsLayout[vsSelect];

		streamDecl = renderDevice->streamDeclCache.buildStreamDecl(
			layoutA,
			layoutB,
			(flags >> 3) & 1,
			&vertexShader->m_sub);
	}

	mg_streamDecl = streamDecl;

	mg_matInstance = matInstance;
	mg_material = this;
	mg_cbdata = drawableExtDword50;
	mg_vsSelectAndFlags = vsSelectAndFlags;
	mg_tesselate = tesselate;

	return mg_streamDecl;
}

PCDX11StreamDecl *PCDX11Material::buildStreamDecl7(
	MaterialInstanceData *matInstance,
	void *drawableExtDword50,
	uint32_t vsSelect,
	VertexAttributeLayoutA *layout,
	uint8_t flags,
	float floatX,
	float floatY)
{
	// TODO
	// return nullptr;

	auto *stateManager = deviceManager->getStateManager();
	uint16_t renderTargetWriteMask = materialBlob->renderTargetWriteMask;
	stateManager->setRenderTargetWriteMask(renderTargetWriteMask & 7);

	return buildStreamDecl015(
		matInstance,
		drawableExtDword50,
		vsSelect,
		0,
		layout,
		flags,
		floatX,
		floatY);
}

}
