#pragma once
#include "rendering/CommonMaterial.h"
#include "PCRenderExternalResource.h"

namespace cdc {

class PCRenderDevice;

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
};

}
