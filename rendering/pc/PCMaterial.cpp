#include "rendering/MaterialData.h"
#include "PCDeviceManager.h"
#include "PCMaterial.h"
#include "shaders/PCShaderLib.h"
#include "shaders/PCShaderManager.h"
#include "surfaces/PCTexture.h"
#include "PCRenderDevice.h"
#include "PCStateManager.h"
#include "PCStreamDecl.h"

namespace cdc {

PCMaterial::PCMaterial(PCRenderDevice *renderDevice) :
	CommonMaterial(),
	PCRenderExternalResource(renderDevice)
{
	// TODO
}

void PCMaterial::load(MaterialBlob *newBlob) {
	// TODO
	materialBlob = newBlob;
}

void PCMaterial::Release() {
	// TODO
}

void PCMaterial::method_18() {
	// TODO
}

void PCMaterial::SetupVertexConstantsAndTextures(
	uint32_t subMaterialIndex,
	MaterialBlobSub* subMat,
	MaterialInstanceData *matInstance,
	char *cbData,
	bool doEverything)
{
	auto *stateManager = deviceManager9->getStateManager();
	auto *texref = subMat->vsTextureRef;

	if (doEverything) {
		if (subMat->vsBufferSize) // up to 32 rows (25..56)
			stateManager->SetPixelShaderConstantF(25, (float*)subMat->vsBufferData, subMat->vsBufferSize);

		// assign 0..refIndexEndA from submaterial
		for (uint32_t i = 0; i < subMat->vsRefIndexEndA; i++)
			renderDevice->setTexture(
				texref[i].slotIndex + 257,
				static_cast<PCTexture*>(texref[i].tex),
				texref[i].filter,
				texref[i].unknown4);

		// assign refIndexEndA..refIndexBeginB from submaterial or RenderGlobalState
		CommonScene *scene = renderDevice->scene78;
		for (uint32_t i = subMat->vsRefIndexEndA; i < subMat->vsRefIndexBeginB; i++) {
			auto fi = texref[i].fallbackIndex & 0x1F;
			PCTexture* tex = nullptr; // = static_cast<PCTexture*>(scene->globalState.tex14[fi]);
			if (!tex) tex = static_cast<PCTexture*>(texref[i].tex);
			renderDevice->setTexture(
				texref[i].slotIndex + 257,
				tex,
				fi == 10 ? kTextureFilterPoint : texref[i].filter,
				texref[i].unknown4);
		}
	}

	// assign refIndexBeginB..refIndexEndB from submaterial or MaterialInstanceData
	for (uint32_t i = subMat->vsRefIndexBeginB; i < subMat->vsRefIndexEndB; i++) {
		auto extTextures = (PCTexture**)matInstance;
		auto fi = texref[i].fallbackIndex & 0x1F;
		PCTexture* tex = nullptr; // = extTextures[fi - 1];
		if (!tex) tex = static_cast<PCTexture*>(texref[i].tex);
		renderDevice->setTexture(
			texref[i].slotIndex + 257,
			tex,
			texref[i].filter,
			texref[i].unknown4);
	}

	if (subMat->vsBufferNumRows) {
		auto firstRow = subMat->vsBufferFirstRow;
		auto numRows = subMat->vsBufferNumRows;
		if (cbData)
			stateManager->SetVertexShaderConstantF(firstRow + 57, (float*)&cbData[16 * firstRow], numRows);
	}
}

void PCMaterial::SetupPixelConstantsAndTextures(
	uint32_t subMaterialIndex,
	MaterialBlobSub* subMat,
	MaterialInstanceData *matInstance,
	char *cbData,
	bool doEverything)
{
	auto *stateManager = deviceManager9->getStateManager();
	auto *texref = subMat->psTextureRef;

	if (doEverything) {
		if (subMat->psBufferSize) // up to 32 rows (100..131)
			stateManager->SetPixelShaderConstantF(100, (float*)subMat->psBufferData, subMat->psBufferSize);

		// assign 0..refIndexEndA from submaterial
		for (uint32_t i = 0; i < subMat->psRefIndexEndA; i++)
			renderDevice->setTexture(
				texref[i].slotIndex,
				static_cast<PCTexture*>(texref[i].tex),
				texref[i].filter,
				texref[i].unknown4);

		// assign refIndexEndA..refIndexBeginB from submaterial or RenderGlobalState
		CommonScene *scene = renderDevice->scene78;
		for (uint32_t i = subMat->psRefIndexEndA; i < subMat->psRefIndexBeginB; i++) {
			auto fi = texref[i].fallbackIndex & 0x1F;
			PCTexture* tex = nullptr; // = static_cast<PCTexture*>(scene->globalState.tex14[fi]);
			if (!tex) tex = static_cast<PCTexture*>(texref[i].tex);
			renderDevice->setTexture(
				texref[i].slotIndex,
				tex,
				fi == 10 ? kTextureFilterPoint : texref[i].filter,
				texref[i].unknown4);
		}
	}

	// assign refIndexBeginB..refIndexEndB from submaterial or MaterialInstanceData
	for (uint32_t i = subMat->psRefIndexBeginB; i < subMat->psRefIndexEndB; i++) {
		auto extTextures = (PCTexture**)matInstance;
		auto fi = texref[i].fallbackIndex & 0x1F;
		PCTexture* tex = nullptr; // = extTextures[fi - 1];
		if (!tex) tex = static_cast<PCTexture*>(texref[i].tex);
		renderDevice->setTexture(
			texref[i].slotIndex,
			tex,
			texref[i].filter,
			texref[i].unknown4);
	}

	if (subMat->psBufferNumRows) {
		auto firstRow = subMat->psBufferFirstRow;
		auto numRows = subMat->psBufferNumRows;
		if (cbData)
			stateManager->SetPixelShaderConstantF(firstRow + 132, (float*)&cbData[16 * firstRow], numRows);
	}
}

PCStreamDecl *PCMaterial::SetupNormalMapPass(
	MaterialInstanceData& data,
	void *instanceParams, // float4
	uint32_t modelType,
	VertexDecl *pVertexDecl,
	uint32_t flags,
	float opacityMultiplier)
{
	auto *stateManager = deviceManager9->getStateManager();

	uint32_t matDword18 = materialBlob->dword18;
	bool frontCounterClockwise = bool(flags & 2);
	bool matInstanceDoubleSided = bool(data.polyFlags & 0x40);
	bool materialDoubleSided = bool(matDword18 & 0x80);
	bool materialRenderTwice = bool(matDword18 & 0x800);
	bool materialCullFront = bool(matDword18 & 0x2000);

	if ((materialDoubleSided || matInstanceDoubleSided) && !materialRenderTwice)
		stateManager->SetCullMode(CullMode::none, frontCounterClockwise);
	else if (materialCullFront)
		stateManager->SetCullMode(CullMode::front, frontCounterClockwise);
	else
		stateManager->SetCullMode(CullMode::back, frontCounterClockwise);

	uint32_t subMaterialIndex = 7; // normals
	MaterialBlobSub *subMaterial = materialBlob->subMat4C[subMaterialIndex];

	bool materialChange = true;
	SetupVertexConstantsAndTextures(subMaterialIndex, subMaterial, &data, (char*)instanceParams, materialChange);
	SetupPixelConstantsAndTextures(subMaterialIndex, subMaterial, &data, (char*)instanceParams, materialChange);

	if (true)
	{
		// set pixel shader
		uint32_t pixelIndex = 0;
		auto pixelLib = static_cast<PCShaderLib*>(subMaterial->shaderPixel);
		if (!pixelLib)
			return nullptr; // HACK
		auto *pixelTable = static_cast<PCPixelShaderTable*>(pixelLib->table);
		auto pixelShader = (*pixelTable)[pixelIndex];
		stateManager->setPixelShader(pixelShader);

		// set vertex shader
		uint32_t vertexIndex = modelType;
		if (flags & 8)
			vertexIndex |= 8;
		auto vertexLib = static_cast<PCShaderLib*>(subMaterial->shaderVertex);
		auto vertexTable = static_cast<PCVertexShaderTable*>(vertexLib->table);
		auto vertexShader = (*vertexTable)[vertexIndex];
		stateManager->setVertexShader(vertexShader);

		auto *streamDecl = static_cast<PCStreamDecl*>(data.streamDecls24[subMaterialIndex]);
		if (!streamDecl) {
			ShaderInputSpec *inputSpec = subMaterial->vsLayout[modelType];
			streamDecl = renderDevice->streamDeclManager.FindOrCreate(
				pVertexDecl,
				inputSpec,
				(flags >> 3) & 1 // flag 8 added in PCTerrainDrawable constructor for example
			);
		}

		return streamDecl;
	}
}

}
