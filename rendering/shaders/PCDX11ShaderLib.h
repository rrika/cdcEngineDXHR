#pragma once
#include "../PCDX11RenderExternalResource.h"
#include "IShaderLib.h"
#include "PCDX11ShaderTable.h"

namespace cdc {

class PCDX11RenderDevice;

class PCDX11ShaderLib :
	public IShaderLib,
	public PCDX11RenderExternalResource
{
public:
	enum Type {
		kPixel = 0x1,
		kVertex = 0x2,
		kHull = 0x3,
		kDomain = 0x4,
		kCompute = 0x5
	};

	PCDX11ShaderLib(uint32_t size, PCDX11RenderDevice *renderDevice) :
		IShaderLib(),
		PCDX11RenderExternalResource(renderDevice),
		table(nullptr)
	{
		buffer = new char[size];
	}

	PCDX11ShaderLib(char *blob, Type type, PCDX11RenderDevice *renderDevice, bool wineWorkaround=false);

	PCDX11ShaderTable *table; // 10
	char *buffer; // 14

	~PCDX11ShaderLib() override = default;
	void fill(char *buffer, uint32_t offset, uint32_t size, bool done) override;
	void Release() override;
};

}
