#pragma once
#include "CommonMaterial.h"
#include "PCDX11RenderExternalResource.h"
#include "surfaces/PCDX11BitmapTexture.h"
// #include "buffers/PCDX11StaticConstantBuffer.h"
#include "buffers/PCDX11UberConstantBuffer.h"

namespace cdc {

class PCDX11StreamDecl;
struct MeshTab0Ext128Sub10;
struct VertexAttributeLayoutA;

class PCDX11Material :
	public CommonMaterial,
	public PCDX11RenderExternalResource
{
	unsigned short numTextures = 0; // 1C
	unsigned short texBits = 0; // 1E
	// PCDX11BitmapTexture *texture[4]; // 20
	// PCDX11StaticConstantBuffer *constantBuffersPs[16]; // 30
	// PCDX11StaticConstantBuffer *constantBuffersVs[16]; // 70
	PCDX11UberConstantBuffer *constantBuffersPs[16]; // 30
	PCDX11UberConstantBuffer *constantBuffersVs[16]; // 70
public:
	PCDX11Material(PCDX11RenderDevice *renderDevice) :
		PCDX11RenderExternalResource(renderDevice)
	{}

	void load(MaterialBlob *) override;
	void method_08() override;
	~PCDX11Material() = default;
	void method_18() override;

	void setupVertexResources(uint32_t, MaterialBlobSub*, MeshTab0Ext128Sub10*, char*, bool);
	void setupPixelResources(uint32_t, MaterialBlobSub*, MeshTab0Ext128Sub10*, char*, bool);

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
		void *drawableDword24,
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
