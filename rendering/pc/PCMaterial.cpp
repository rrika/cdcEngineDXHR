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

PCStreamDecl *PCMaterial::SetupNormalMapPass(
	MaterialInstanceData& data,
	void *instanceParams, // float4
	uint32_t modelType,
	VertexDecl *pVertexDecl,
	uint32_t flags,
	float opacityMultiplier)
{
	auto *stateManager = deviceManager9->getStateManager();

	uint32_t subMaterialIndex = 7; // normals
	MaterialBlobSub *subMaterial = materialBlob->subMat4C[subMaterialIndex];

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
