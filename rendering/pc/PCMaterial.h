#pragma once
#include "rendering/CommonMaterial.h"
#include "PCRenderExternalResource.h"

namespace cdc {

class PCRenderDevice;
class PCStreamDecl;
class PCStreamDeclManager;
struct MaterialBlobSub; // = cdc::PassData
struct VertexDecl;

class PCMaterial :
	public CommonMaterial,
	public PCRenderExternalResource
{
public:
	PCMaterial(PCRenderDevice*);

	void load(MaterialBlob*) override;
	void Release() override;
	void method_18() override;

	void SetupVertexConstantsAndTextures(uint32_t, MaterialBlobSub*, MaterialInstanceData*, char*, bool);
	void SetupPixelConstantsAndTextures(uint32_t, MaterialBlobSub*, MaterialInstanceData*, char*, bool);

	~PCMaterial() = default;

	PCStreamDecl *PrepStreamDecl(
		MaterialInstanceData& data,
		uint32_t modelType,
		uint32_t passIndex,
		bool withNormals,
		VertexDecl *pVertexDecl,
		PCStreamDeclManager& streamDeclManager);

	PCStreamDecl *SetupNormalMapPass(
		MaterialInstanceData& data,
		void *instanceParams, // float4
		uint32_t modelType,
		VertexDecl *pVertexDecl,
		uint32_t flags,
		float opacityMultiplier);
};

}
