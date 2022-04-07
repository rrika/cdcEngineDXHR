#pragma once
#include "PCDX11InternalResource.h"
#include "CommonRenderDevice.h"

namespace cdc {

class PCDX11RenderDevice :
	public PCDX11InternalResource,
	public CommonRenderDevice
{
public:
	// incomplete
	PCDX11RenderDevice();
	void refCountDec();
	void method_08();
	void method_0C();
	void method_10();
	void method_1C();
	void method_20();
	void method_24();
	void method_28();
	void method_2C();
	void method_30();
	void method_48();
	void method_50();
	void getSceneRenderTarget();
	void method_64();
	void method_6C();
	void method_70();
	void method_A8();
	void clearRenderTarget();
	void setRenderTarget();
	void method_C0();
	void method_C4();
	void method_C8();
	void method_CC();
	void method_D0();
	void method_D4();
	void method_D8();
	void method_DC();
	void method_EC();
	void createMaterial();
	TextureMap *createTexture(uint32_t);
	void createProceduralTexture();
	void createShader();
	RenderResource *createRenderModel(uint32_t);
	void createRenderModelInstance();
	RenderResource *createRenderTerrain(uint32_t);
	void createRenderTerrainInstance();
	void createRenderImage();
	void createWaterSurface();
	void createLightResource();
	void createRenderLight();
	void createMatrixState();
	void createVertexBuffer();
	void createIndexBuffer();
	void method_16C();
	void createDepthBuffer();
	void method_174();
	void getWindowSize(uint *width, uint *height);
	void method_17C();
	void method_180();

	void internalResource04();
	void internalResource08();
};

PCDX11RenderDevice *createPCDX11RenderDevice(HWND hwnd, uint width, uint height, bool unknown);

}
