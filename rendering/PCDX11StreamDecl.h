#pragma once
#include <cstdint>
#include <d3d11.h>
#include "PCDX11InternalResource.h"
#include "CommonStreamDecl.h"
#include "shaders/PCDX11Shader.h"

class D3D11_INPUT_ELEMENT_DESC;

namespace cdc {

class PCDX11ConstantBuffer;
class PCDX11RenderDevice;
class PCDX11VertexBuffer;

class PCDX11StreamDecl :
	public PCDX11InternalResource,
	public CommonStreamDecl
{
public:
	PCDX11StreamDecl(
		PCDX11RenderDevice *renderDevice,
		D3D11_INPUT_ELEMENT_DESC *elementDesc,
		uint32_t numElements,
		PCDX11ShaderBinary *shaderSub)
	:
		PCDX11InternalResource(),
		renderDevice(renderDevice),
		elementDesc(elementDesc),
		numElements(numElements),
		inputLayout(nullptr),
		shaderSub(shaderSub),
		secondaryData(nullptr),
		vertexBuffer(nullptr),
		streamDeclBuffer(nullptr)
	{
		// InternalResource::...
	}

	~PCDX11StreamDecl();

	PCDX11RenderDevice *renderDevice; // 10
	D3D11_INPUT_ELEMENT_DESC *elementDesc; // 1C
	uint32_t numElements; // 20
	ID3D11InputLayout *inputLayout; // 24
	PCDX11ShaderBinary *shaderSub; // 2C
	Vector4 *secondaryData; // 30
	PCDX11VertexBuffer *vertexBuffer; // 34 secondary vertex stream
	PCDX11ConstantBuffer *streamDeclBuffer; // 38

	void apply();

	bool internalCreate() override;
	void internalRelease() override;
};

}
