#pragma once
#include <cstdint>
#include "PCInternalResource.h"
#include "rendering/CommonStreamDecl.h"

struct _D3DVERTEXELEMENT9;
typedef _D3DVERTEXELEMENT9 D3DVERTEXELEMENT9;
struct IDirect3DVertexDeclaration9;

namespace cdc {

class PCStreamDecl :
	public PCInternalResource,
	public CommonStreamDecl
{
public:
	PCStreamDecl(
		D3DVERTEXELEMENT9 *elements)
	:
		elements(elements)
	{
		// TODO
	}

	D3DVERTEXELEMENT9 *elements; // 18
	IDirect3DVertexDeclaration9 *vertexDecl; // 1C

	void apply();

	bool internalCreate() override;
	void internalRelease() override;
};

}
