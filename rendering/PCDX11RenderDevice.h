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
	void setupPassCallbacks();
	void registerComparatorsAndDrawersModel();
	void registerComparatorsAndDrawersTerrain1();
	void registerComparatorsAndDrawersTerrain2();

	// incomplete
	void refCountDec() override;
	void method_08() override;
	void method_0C() override;
	void method_10() override;
	void method_1C() override;
	void method_20() override;
	void method_24() override;
	void method_28() override;
	void method_2C() override;
	void method_30() override;
	CommonScene *createScene(
		CommonSceneSub18 *sub18,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer) override;
	void method_50() override;
	void getSceneRenderTarget() override;
	void method_64() override;
	void method_6C() override;
	void method_70() override;
	void method_A8() override;
	void clearRenderTarget(
		uint32_t flags,
		uint32_t unknown1,
		float unknown2,
		float *clearColor,
		float clearDepth,
		uint32_t clearStencil) override;
	void setRenderTarget() override;
	void method_C0() override;
	void method_C4() override;
	void method_C8() override;
	void method_CC() override;
	void method_D0() override;
	void method_D4() override;
	void method_D8() override;
	void method_DC() override;
	void method_EC() override;
	void createMaterial() override;
	TextureMap *createTexture(uint32_t) override;
	void createProceduralTexture() override;
	IShaderLib *createShaderLib(uint32_t) override;
	RenderResource *createRenderModel(uint32_t) override;
	void createRenderModelInstance() override;
	RenderResource *createRenderTerrain(uint32_t) override;
	void createRenderTerrainInstance() override;
	void createRenderImage() override;
	void createWaterSurface() override;
	void createLightResource() override;
	void createRenderLight() override;
	void createMatrixState() override;
	void createVertexBuffer() override;
	void createIndexBuffer() override;
	void method_16C() override;
	void createDepthBuffer() override;
	void method_174() override;
	void getWindowSize(uint *width, uint *height) override;
	void method_17C() override;
	void method_180() override;

	bool internalCreate() override;
	void internalRelease() override;

	virtual void dx11_method_0();
	virtual void dx11_method_4();
	virtual void dx11_method_8();
	virtual void dx11_method_C();
	virtual void dx11_method_10();
	virtual void dx11_method_14();
	virtual void dx11_method_18();
	virtual void dx11_method_1C();
	virtual void dx11_method_20();
	virtual void dx11_method_24();
	virtual void dx11_method_28();
	virtual void dx11_method_2C();
	virtual void dx11_method_30();
	virtual void dx11_method_34();
	virtual void dx11_method_38();
	virtual void dx11_method_3C();
	virtual void dx11_method_40();
	virtual void dx11_method_44();
	virtual void dx11_method_48();

	ID3D11DeviceContext *d3dDeviceContext111580;
	ID3D11DeviceContext *getD3DDeviceContext() { return d3dDeviceContext111580; }
	void recordDrawable(IRenderDrawable *drawable, uint32_t arg1, uint8_t arg2);
	void clearRenderTargetNow(char mode, float *color, float depth, uint32_t stencil);
};

PCDX11RenderDevice *createPCDX11RenderDevice(HWND hwnd, uint width, uint height, bool unknown);

}
