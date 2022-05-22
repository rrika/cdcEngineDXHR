#pragma once
#include "CommonMaterial.h"
#include "PCDX11BitmapTexture.h"
#include "PCDX11StaticConstantBuffer.h"

namespace cdc {

class PCDX11StreamDecl;
struct MeshTab0Ext128Sub10;
struct VertexAttributeLayoutA;

class PCDX11Material :
	public CommonMaterial
	// public PCDX11RenderExternalResource
{
	unsigned short numTextures = 0;
	unsigned short texBits = 0;
	// PCDX11BitmapTexture *texture[4];
	// PCDX11StaticConstantBuffer *constantBuffersPs[16];
	// PCDX11StaticConstantBuffer *constantBuffersVs[16];
public:
	PCDX11Material() {}

	void method_04() override;
	void method_0C() override;
	void method_10() override;
	~PCDX11Material() = default;
	void method_18() override;

	PCDX11StreamDecl *buildStreamDecl015(
		MeshTab0Ext128Sub10*,
		void *drawableExtDword50,
		uint32_t vsSelect,
		bool arg4,
		VertexAttributeLayoutA *layout,
		uint8_t flags,
		float floatX,
		float floatY);

	PCDX11StreamDecl *buildStreamDecl01(
		MeshTab0Ext128Sub10*,
		void *drawableExtDword50,
		uint8_t lightManager434_114,
		uint32_t vsSelect,
		VertexAttributeLayoutA *layout,
		uint8_t flags,
		float floatX,
		float floatY);

	PCDX11StreamDecl *buildStreamDecl4(
		MeshTab0Ext128Sub10*,
		void *drawableExtDword50,
		uint32_t vsSelect,
		VertexAttributeLayoutA *layout,
		uint8_t flags,
		float floatX);

	PCDX11StreamDecl *buildStreamDecl038(
		MeshTab0Ext128Sub10*,
		void *drawableExtDword50,
		uint32_t drawableDword24,
		uint32_t vsSelect,
		VertexAttributeLayoutA *layout,
		uint8_t flags,
		bool flag,
		float floatX,
		float floatY);

	PCDX11StreamDecl *buildStreamDecl7(
		MeshTab0Ext128Sub10*,
		void *drawableExtDword50,
		uint32_t vsSelect,
		VertexAttributeLayoutA *layout,
		uint8_t flags,
		float floatX,
		float floatY);
};

}
