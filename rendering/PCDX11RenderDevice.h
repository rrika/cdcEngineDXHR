#pragma once
#include <d3d11.h>
#include "CommonRenderDevice.h"
#include "PCDX11InternalResource.h"
#include "PCDX11StreamDeclCache.h"
#include "VertexDeclaration.h"
#include "shaders/PCDX11ShaderLib.h"
#include "shaders/PCDX11ShaderTable.h" // for PixelShaderTable and VertexShaderTable
#include "surfaces/PCDX11DepthBuffer.h" // for covariant return type
#include "surfaces/PCDX11RenderTarget.h" // for covariant return type

namespace cdc {

class IRenderDrawable;
class PCDX11BaseTexture;
class PCDX11LightManager;
class PCDX11Pool;
class PCDX11RenderContext;
class PCDX11RenderTexture;
class PCDX11StaticPool;
class PCDX11SimpleDynamicVertexBuffer;
class PCDX11SimpleStaticVertexBuffer;
class PCDX11Texture;
struct LightManagerSubB;

class PCDX11RenderDevice :
	public PCDX11InternalResource,
	public CommonRenderDevice
{
public:
	struct RenderList {
		RenderList(PCDX11RenderDevice *renderDevice, uint32_t *dimensions);

		uint32_t dword0;
		uint32_t dword4;
		uint32_t widthMaybe8;
		uint32_t heightMaybeC;
		PCDX11RenderTarget *renderTarget; // 10
		PCDX11DepthBuffer *depthBuffer; // 14
		DrawableList drawableList; // 18
		LightManagerSubB *lightManagerSubB; // 28
		// uint8_t byte2C;
		// uint8_t byte2D;
		RenderList *next;
	};

	PCDX11RenderContext *renderContext_10CE8 = nullptr;
	PCDX11RenderContext *renderContext_10CEC;

	RenderList *renderList_current = nullptr; // 10CF8
	RenderList *renderList_processing = nullptr; // 10CFC
	RenderList *renderList_first = nullptr; // 10D00
	RenderList *renderList_last = nullptr; // 10D04
	RenderList *renderList_override = nullptr; // own addition

	PCDX11Pool *m_pDynamicVertexPool = nullptr; // 10D08
	PCDX11Pool *m_pDynamicIndexPool = nullptr; // 10D0C
	PCDX11StaticPool *m_pStaticVertexPool = nullptr; // 10D10
	PCDX11StaticPool *m_pStaticIndexPool = nullptr; // 10D14

	VertexDecl *drawVertexDecls[7] = {}; // 10BBC

	PCDX11StreamDecl *vertex2DStreamDecl; // 10BDC, assigned in createDefaultVertexAttribLayouts
	PCDX11StreamDecl *position3DStreamDecl;  // 10BE0, assigned in createDefaultVertexAttribLayouts

	PCDX11ShaderLib *shlib_22; // 10BE4, ps
	PCDX11ShaderLib *shlib_21; // 10BE8, vs
	PCDX11ShaderLib *shlib_20; // 10BEC, vs
	PCDX11ShaderLib *shlib_19; // 10BF0, vs
	PCDX11ShaderLib *shlib_18; // 10BF4, vs (copy surface)
	PCDX11ShaderLib *shlib_17; // 10BF8, ps
	PCDX11ShaderLib *shlib_16; // 10BFC, ps
	PCDX11ShaderLib *shlib_14; // 10C00, vs
	PCDX11ShaderLib *shlib_15; // 10C04, vs
	PCDX11ShaderLib *shlib_13; // 10C08, ps (mlaa step 1)
	PCDX11ShaderLib *shlib_11; // 10C0C, ps (mlaa step 2)
	PCDX11ShaderLib *shlib_10; // 10C10, ps (mlaa step 3)
	PCDX11ShaderLib *shlib_12; // 10C14, ps (fxaa, all qualities)
	PCDX11ShaderLib *shlib_9;
	PCDX11ShaderLib *shlib_8;
	PCDX11ShaderLib *shlib_7;
	PCDX11ShaderLib *shlib_6;
	PCDX11ShaderLib *shlib_5;
	PCDX11ShaderLib *shlib_4; // 10C2C, cs (fast blur)
	PCDX11ShaderLib *shlib_3; // 10C30, cs (fast blur)
	PCDX11ShaderLib *shlib_2; // 10C34, cs (bilateral blur)
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

	PCDX11StreamDeclCache streamDeclCache; // 1112D4
	PCDX11Texture *missingTexture; // 1112F0

	PCDX11SimpleStaticVertexBuffer *fullScreenQuadVB; // 11156C
	PCDX11SimpleDynamicVertexBuffer *quadVB; // 111570

	ID3D11DeviceContext *deviceContext; // 111580

public:
	PCDX11RenderDevice(HWND hwnd, uint32_t width, uint32_t height);
	void createDefaultResources();
	void createDefaultVertexAttribLayouts();
	void setupPassCallbacks();
	void registerComparatorsAndDrawersModel();
	void registerComparatorsAndDrawersTerrain();
	void registerComparatorsAndDrawersNGAPrim();
	~PCDX11RenderDevice();
	void setupShadowBuffer();

	// incomplete
	void refCountDec() override;
	void method_08() override;
	void resetRenderLists(float timeDelta) override;
	void drawRenderLists() override;
	bool beginRenderList(float*) override;
	bool endRenderList() override;
	bool hasRenderList() override;
	uint32_t getSubFrameWidth() override;
	uint32_t getSubFrameHeight() override;
	void method_30() override;
	CommonScene *createSubScene(
		RenderViewport *renderViewport,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		CommonRenderTarget *sourceColor,
		CommonDepthBuffer *sourceDepth) override;
	void finishScene() override;
	PCDX11RenderTarget *getSceneRenderTarget() override; // covariant
	PCDX11DepthBuffer *getSceneDepthBuffer() override; // covariant
	uint32_t getContextWidth() override;
	uint32_t getContextHeight() override;
	void method_A8() override;
	void clearRenderTarget(
		uint32_t flags,
		uint32_t unknown1,
		float unknown2,
		float *clearColor,
		float clearDepth,
		uint32_t clearStencil) override;
	void setRenderTarget() override;
	void DrawPrimitive(Matrix*, TextureMap*, RenderVertex*, uint32_t, uint32_t, float) override;
	void DrawIndexedPrimitive(PrimitiveContext*, uint32_t, uint32_t, float) override;
	void DrawIndexedPrimitive(PrimitiveContext*, void*, VertexDecl*, uint32_t, uint16_t*, uint32_t, float) override;
	void method_CC() override;
	void method_D0() override;
	void method_D4() override;
	void method_D8() override;
	void method_DC() override;
	void method_EC() override;
	IMaterial *createMaterial() override;
	TextureMap *createTexture(uint32_t) override;
	void createProceduralTexture() override;
	IShaderLib *createShaderLib(uint32_t) override;
	RenderMesh *createRenderModel(uint32_t) override;
	RenderModelInstance *createRenderModelInstance(RenderMesh*) override;
	IRenderTerrain *createRenderTerrain(uint32_t) override;
	IRenderTerrainInstance *createRenderTerrainInstance(IRenderTerrain*) override;
	void createRenderImage() override;
	void createWaterSurface() override;
	void createLightResource() override;
	void createRenderLight() override;
	IMatrixState *createMatrixState() override;
	void createVertexBuffer() override;
	void createIndexBuffer() override;
	IRenderTarget *createRenderTarget(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, TextureClass) override;
	IDepthBuffer *createDepthBuffer() override;
	void method_174() override;
	void getWindowSize(uint32_t *width, uint32_t *height) override;
	void method_17C() override;
	void method_180() override;

	// our own additions
	void *captureRenderLists() override;
	void revisitRenderLists(void*) override;
	void freeRenderLists(void*) override;

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
	virtual PCDX11RenderContext *getRenderContextAny();
	virtual void dx11_method_24();
	virtual void dx11_method_28();
	virtual void dx11_method_2C();
	virtual void dx11_method_30();
	virtual PCDX11RenderTarget *dx11_createRenderTarget(
		uint32_t width,
		uint32_t height,
		uint32_t dxgiFormat,
		uint32_t,
		TextureClass);
	virtual void dx11_method_38();
	virtual void dx11_method_3C();
	virtual void dx11_method_40();
	virtual void handleResize(int32_t width, int32_t height);
	virtual void dx11_method_48();

	ID3D11DeviceContext *d3dDeviceContext111580;
	ID3D11DeviceContext *getD3DDeviceContext() { return d3dDeviceContext111580; }
	PCDX11LightManager *getLightManager();
	PCDX11RenderTarget *GetDefaultRenderTarget(bool isFlush);
	PCDX11DepthBuffer *GetDefaultDepthBuffer(bool isFlush);
	void recordDrawable(IRenderDrawable *drawable, uint32_t mask, bool addToNextScene);
	void clearRenderTargetNow(char mode, float *color, float depth, uint32_t stencil);
	void setTexture(uint32_t slot, PCDX11BaseTexture *tex, uint32_t filter, float unknown);
	void drawRenderListsInternal(void *arg);
	void drawQuadInternal(
		float x0, float y0, float x1, float y1,
		float u0, float v0, float u1, float v1,
		uint32_t color);
	void drawQuad(
		float x0, float y0, float x1, float y1,
		float u0, float v0, float u1, float v1,
		uint32_t color, uint32_t flags, uint32_t blendMode, bool writeDepth);
	void copySurface(PCDX11RenderTexture *texture, bool writeDepth, uint8_t rtMask);
	void ApplyFXAA(uint32_t quality, PCDX11BaseTexture *texture, PCDX11RenderTarget *renderTarget);
	void ApplyMLAA(PCDX11BaseTexture *texture, PCDX11RenderTarget *renderTarget);
	void fastBlur(
		PCDX11BaseTexture *texture, PCDX11RenderTarget *renderTarget,
		uint32_t passMask, bool isHorizontal, bool weighted);
	PCDX11ComputeShader *getBlurShader(bool horizontal, uint32_t kind);
};

CommonRenderDevice *createPCDX11RenderDevice(HWND hwnd, uint32_t width, uint32_t height, bool unknown);

}
