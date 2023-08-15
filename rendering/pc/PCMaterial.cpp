#include "rendering/MaterialData.h"
#include "PCDeviceManager.h"
#include "PCMaterial.h"
#include "shaders/PCShaderLib.h"
#include "shaders/PCShaderManager.h"
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

	// TODO

	if (subMat->vsBufferNumRows) {
		auto firstRow = subMat->vsBufferFirstRow;
		auto numRows = subMat->vsBufferNumRows;
		if (cbData)
			stateManager->SetVertexShaderConstantF(firstRow + 132, (float*)&cbData[16 * firstRow], numRows);
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

	// TODO

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
