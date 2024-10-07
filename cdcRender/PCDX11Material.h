#pragma once
#include "CommonMaterial.h"
#include "PCDX11RenderExternalResource.h"
#include "surfaces/PCDX11BitmapTexture.h"
// #include "buffers/PCDX11StaticConstantBuffer.h"
#include "buffers/PCDX11UberConstantBuffer.h"

namespace cdc {

class PCDX11StreamDecl;
struct MaterialBlobSub;
struct MaterialInstanceData;
struct VertexDecl;

class PCDX11Material :
	public CommonMaterial,
	public PCDX11RenderExternalResource
{
	unsigned short numTextures = 0; // 1C
	unsigned short texBits = 0; // 1E
	// PCDX11BitmapTexture *texture[4]; // 20
	// PCDX11StaticConstantBuffer *constantBuffersPs[16]; // 30
	// PCDX11StaticConstantBuffer *constantBuffersVs[16]; // 70
	PCDX11UberConstantBuffer *constantBuffersPs[16] = {0}; // 30
	PCDX11UberConstantBuffer *constantBuffersVs[16] = {0}; // 70

	// mg = material globals
	static uint32_t mg_state; // 00B37BE0
	static uint32_t mg_vsSelectAndFlags;// 00B37BE4

	// these globals might be declared elsewhere
	static PCDX11StreamDecl *mg_streamDecl;     // 00EAAD18
	static VertexDecl *mg_layoutA;  // 00EAAD1C
	static PCDX11Material *mg_material;         // 00EAAD20
	static void *mg_cbdata;                     // 00EAAD24
	static MaterialInstanceData *mg_matInstance; // 00EAAD28
	static bool mg_tesselate;                   // 00EAAD2C

public:
	PCDX11Material(PCDX11RenderDevice *renderDevice) :
		PCDX11RenderExternalResource(renderDevice)
	{}
	void FreeData();

	void load(MaterialBlob *) override;
	void Release() override;
	~PCDX11Material() override;
	void method_18() override;

	void setupVertexResources(uint32_t, MaterialBlobSub*, MaterialInstanceData*, char*, bool);
	void setupPixelResources(uint32_t, MaterialBlobSub*, MaterialInstanceData*, char*, bool);

	void setupDepthBias(MaterialInstanceData*);
	void setupStencil(MaterialInstanceData*, bool, uint32_t);

	void setupSinglePassOpaque(PCDX11RenderDevice*, MaterialInstanceData*, uint32_t);
	void setupSinglePassTranslucent(PCDX11RenderDevice*, MaterialInstanceData*, uint32_t, float);
	static void invalidate();

	PCDX11StreamDecl *SetupDepthPass(
		MaterialInstanceData*,
		void *drawableExtDword50,
		uint32_t vsSelect,
		bool arg4,
		VertexDecl *layout,
		uint8_t flags,
		float opacityMultiplier,
		float floatY);

	PCDX11StreamDecl *SetupShadowPass(
		MaterialInstanceData*,
		void *drawableExtDword50,
		uint8_t lightManager434_114,
		uint32_t vsSelect,
		VertexDecl *layout,
		uint8_t flags,
		float opacityMultiplier,
		float floatY);

	PCDX11StreamDecl *SetupBloomPass(
		MaterialInstanceData*,
		void *drawableExtDword50,
		uint32_t vsSelect,
		VertexDecl *layout,
		uint8_t flags,
		float opacityMultiplier);

	PCDX11StreamDecl *SetupSinglePass(
		MaterialInstanceData*,
		void *drawableExtDword50,
		void *drawableDword24,
		uint32_t vsSelect,
		VertexDecl *layout,
		uint8_t flags,
		bool isTranslucentPass,
		float opacityMultiplier,
		float lodDistance);

	PCDX11StreamDecl *SetupNormalMapPass(
		MaterialInstanceData*,
		void *drawableExtDword50,
		uint32_t vsSelect,
		VertexDecl *layout,
		uint8_t flags,
		float opacityMultiplier,
		float floatY);
};

}
