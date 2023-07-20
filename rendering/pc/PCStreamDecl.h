#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "PCInternalResource.h"
#include "rendering/CommonStreamDecl.h"

struct _D3DVERTEXELEMENT9;
typedef _D3DVERTEXELEMENT9 D3DVERTEXELEMENT9;
struct IDirect3DVertexDeclaration9;

namespace cdc {

class PCVertexBuffer;

class PCStreamDecl :
	public PCInternalResource,
	public CommonStreamDecl
{
public:
	PCStreamDecl(
		D3DVERTEXELEMENT9 *elements,
		uint32_t stride)
	:
		elements(elements),
		stride(stride)
	{
		// TODO
	}

	D3DVERTEXELEMENT9 *elements; // 18
	IDirect3DVertexDeclaration9 *vertexDecl; // 1C
	uint32_t stride; // 20
	Vector4 *secondaryData; // 24
	PCVertexBuffer *secondaryVertexBuffer; // 28
	Vector4 texcoordScales; // 30
	Vector4 normalScaleOffset; // 40

	void apply();

	bool internalCreate() override;
	void internalRelease() override;
};

}
