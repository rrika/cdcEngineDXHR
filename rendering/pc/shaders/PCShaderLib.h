#pragma once
#include "rendering/shaders/IShaderLib.h"
#include "rendering/pc/PCRenderExternalResource.h"
#include "rendering/pc/shaders/PCShaderTable.h"

namespace cdc {

class PCRenderDevice;

class PCShaderLib :
	public IShaderLib,
	public PCRenderExternalResource
{
public:
	PCShaderLib(uint32_t size, PCRenderDevice *renderDevice) :
		IShaderLib(),
		PCRenderExternalResource(renderDevice),
		table(nullptr)
	{
		buffer = new char[size];
	}

	PCShaderLib(char *blob, bool isPixelShader, PCRenderDevice *renderDevice);

	PCShaderTable *table; // 10
	char *buffer; // 14

	~PCShaderLib() override = default;
	void fill(char *buffer, uint32_t offset, uint32_t size, bool done) override;
	// TODO: method8
};

}
