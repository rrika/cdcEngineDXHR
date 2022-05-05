#pragma once
#include <d3d11.h>
#include "CommonRenderDevice.h"
#include "PCDX11InternalResource.h"
#include "PCDX11ShaderLib.h"
#include "PCDX11ShaderTable.h" // for PixelShaderTable and VertexShaderTable

namespace cdc {

class IRenderDrawable;

class PCDX11RenderDevice :
	public PCDX11InternalResource,
	public CommonRenderDevice
{
public:
	PCDX11ShaderLib *shlib_22;
	PCDX11ShaderLib *shlib_21;
	PCDX11ShaderLib *shlib_20;
	PCDX11ShaderLib *shlib_19;
	PCDX11ShaderLib *shlib_18;
	PCDX11ShaderLib *shlib_17;
	PCDX11ShaderLib *shlib_16;
	PCDX11ShaderLib *shlib_15;
	PCDX11ShaderLib *shlib_14;
	PCDX11ShaderLib *shlib_13;
	PCDX11ShaderLib *shlib_12;
	PCDX11ShaderLib *shlib_11;
	PCDX11ShaderLib *shlib_10;
	PCDX11ShaderLib *shlib_9;
	PCDX11ShaderLib *shlib_8;
	PCDX11ShaderLib *shlib_7;
	PCDX11ShaderLib *shlib_6;
	PCDX11ShaderLib *shlib_5;
	PCDX11ShaderLib *shlib_4;
	PCDX11ShaderLib *shlib_3;
	PCDX11ShaderLib *shlib_2;
	PCDX11ShaderLib *shlib_1;
	PCDX11ShaderLib *shlib_0;
	PCDX11VertexShaderTable shtab_vs_wvp_1_0; // 10D1C
	PCDX11VertexShaderTable shtab_vs_ui;      // 10D38
	PCDX11VertexShaderTable shtab_vs_wvp;     // 10D54
	PCDX11PixelShaderTable shtab_ps_white_27; // 10D70
	PCDX11PixelShaderTable shtab_ps_passthrough; // 10D90
	PCDX11PixelShaderTable shtab_ps_white_25; // 10DB0
	PCDX11PixelShaderTable shtab_ps_fogColor; // 10DD0
	PCDX11PixelShaderTable shtab_ps_errorColor; // 10DF0

public:
	PCDX11RenderDevice();
	void createDefaultResources();
	void createDefaultVertexAttribLayouts();

	// incomplete
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
	void clearRenderTarget(
		uint32_t flags,
		uint32_t unknown1,
		float unknown2,
		float *clearColor,
		float clearDepth,
		uint32_t clearStencil);
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
	IShaderLib *createShaderLib(uint32_t);
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

	bool internalCreate();
	void internalRelease();

	ID3D11DeviceContext *d3dDeviceContext111580;
	ID3D11DeviceContext *getD3DDeviceContext() { return d3dDeviceContext111580; }
	void recordDrawable(IRenderDrawable *drawable, uint32_t arg1, uint8_t arg2);
	void clearRenderTargetNow(char mode, float *color, float depth, uint32_t stencil);
};

PCDX11RenderDevice *createPCDX11RenderDevice(HWND hwnd, uint width, uint height, bool unknown);

}
