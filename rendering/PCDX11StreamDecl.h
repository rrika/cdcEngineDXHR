#pragma once
#include <cstdint>
#include <d3d11.h>
#include "PCDX11InternalResource.h"
// #include "CommonStreamDecl.h"
#include "PCDX11Shader.h"

class D3D11_INPUT_ELEMENT_DESC;

namespace cdc {

class PCDX11RenderDevice;
class PCDX11VertexBuffer;

class PCDX11StreamDecl :
	public PCDX11InternalResource
	// public CommonStreamDecl
{
public:
	PCDX11StreamDecl(
		PCDX11RenderDevice *renderDevice,
		D3D11_INPUT_ELEMENT_DESC *elementDesc,
		uint32_t numElements,
		PCDX11ShaderSub *shaderSub)
	:
		PCDX11InternalResource(),
		renderDevice(renderDevice),
		elementDesc(elementDesc),
		numElements(numElements),
		inputLayout(nullptr),
		shaderSub(shaderSub),
		vertexBuffer(nullptr)
	{
		// InternalResource::...
	}

	PCDX11RenderDevice *renderDevice; // 10
	D3D11_INPUT_ELEMENT_DESC *elementDesc; // 1C
	uint32_t numElements; // 20
	ID3D11InputLayout *inputLayout; // 24
	PCDX11ShaderSub *shaderSub; // 2C
	PCDX11VertexBuffer *vertexBuffer; // 34 secondary vertex stream

	void apply();

	bool internalCreate() override;
	void internalRelease() override;
};

}
