#pragma once
#include "rendering/CommonMaterial.h"
#include "PCRenderExternalResource.h"

namespace cdc {

class PCRenderDevice;
class PCStreamDecl;
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

	~PCMaterial() = default;

	PCStreamDecl *SetupNormalMapPass(
		MaterialInstanceData& data,
		void *instanceParams, // float4
		uint32_t modelType,
		VertexDecl *pVertexDecl,
		uint32_t flags,
		float opacityMultiplier);
};

}
