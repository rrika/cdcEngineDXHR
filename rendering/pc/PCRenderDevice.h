#pragma once
#include <windows.h>
#include "rendering/CommonRenderDevice.h"
#include "rendering/pc/PCInternalResource.h"
#include "rendering/pc/PCRenderContext.h"
#include "rendering/pc/PCStreamDeclManager.h"
#include "rendering/Types.h"

namespace cdc {

class PCBaseTexture;

class PCRenderDevice :
	public PCInternalResource,
	public CommonRenderDevice
{
	struct RenderList {
		RenderList(PCRenderDevice *renderDevice) :
			drawableList { renderDevice->getLinear() }
		{}

		DrawableList drawableList; // 18
		RenderList *next; // 30
	};

	PCRenderContext *renderContext = nullptr; // 10C9C

	RenderList *renderList_current = nullptr; // 10CAC
	RenderList *renderList_processing = nullptr; // 10CB0
	RenderList *renderList_first = nullptr; // 10CB4
	RenderList *renderList_last = nullptr; // 10CB8
	RenderList *renderList_override = nullptr; // own addition

public:
	PCStreamDeclManager streamDeclManager { this }; // 111288
private:

	PCRenderContext	*CreateRenderContext(HWND hwnd, uint32_t width, uint32_t height, bool useMultiSample);

public:
	PCRenderDevice(HWND hwnd, uint32_t width, uint32_t height);
	void registerComparatorsAndDrawersModel();

	void refCountDec() override;
	void method_08() override;
	void resetRenderLists() override;
	void drawRenderLists() override; // 10
	// inherit method_14
	// inherit method_18
	bool beginRenderList(float*) override;
	bool endRenderList() override; // 20
	bool hasRenderList() override;
	uint32_t getSubFrameWidth() override;
	uint32_t getSubFrameHeight() override;
	void method_30() override;
	// inherit method_34
	// inherit method_38
	// inherit method_3C
	// inherit method_40
	// inherit method_44
	CommonScene *createSubScene(
		RenderViewport *viewport,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer) override;
	// inherit createSiblingScene
	void finishScene() override;
	// inherit method_54
	// inherit method_58
	// inherit getScene
	void getSceneRenderTarget() override;
	void method_64() override;
	// inherit method_68
	void method_6C() override;
	void method_70() override;

	// inherit addPass
	// inherit method_78
	// inherit getPassOrder
	// inherit method_80
	// inherit setPassCallback
	// inherit method_88
	// inherit method_8C

	// inherit allocFuncIndex
	// inherit freeFuncIndex
	// inherit registerDrawer
	// inherit getDrawer
	// inherit registerComparator
	// inherit getComparator

	void method_A8() override;
	// inherit method_AC
	// inherit method_B0
	// inherit method_B4
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
	// inherit method_E0
	// inherit method_E4
	// inherit getGlobalState
	void method_EC() override;
	// inherit method_F0
	void method_F4() override;
	// inherit method_F8
	// inherit method_FC
	// inherit method_100
	// inherit method_104
	void method_108() override;
	// inherit method_10C
	// inherit method_110
	// inherit method_114
	// inherit method_118
	// inherit method_11C
	// inherit method_120
	// inherit method_124
	// inherit linearAlloc30
	IMaterial *createMaterial() override;
	TextureMap *createTexture(uint32_t) override;
	void createProceduralTexture() override;
	IShaderLib *createShaderLib(uint32_t) override;
	RenderMesh *createRenderModel(uint32_t) override;
	// inherit createRenderModelData
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
	IRenderTarget *createRenderTarget(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) override;
	IDepthBuffer *createDepthBuffer() override;
	void method_174() override;
	void getWindowSize(uint32_t *width, uint32_t *height) override;
	void method_17C() override;
	void method_180() override;
	void method_184() override;
	// inherit isInStereoMode
	// inherit method_18C
	// inherit method_190
	// inherit useAlternateLinearAlloc
	void method_198() override;
	void method_19C() override;
	// inherit method_1A0

	// our own additions
	void *captureRenderLists() override;
	void revisitRenderLists(void*) override;
	void freeRenderLists(void*) override;

	bool internalCreate() override;
	void internalRelease() override;

	// virtual void method4() = 0;
	// virtual void method8() = 0;
	// virtual void methodC() = 0;
	// virtual void method10() = 0;
	// virtual void method14() = 0;
	// virtual void method18() = 0;
	virtual PCRenderContext *getRenderContext(); // 1C
	// virtual void method20() = 0;
	// virtual void method24() = 0;
	// virtual void method28() = 0;
	// virtual void method2C() = 0;
	// virtual void method30() = 0;
	// virtual void method34() = 0;
	// virtual void method38() = 0;
	// virtual void method3C() = 0;
	// virtual void method40() = 0;

	void recordDrawable(IRenderDrawable *drawable, uint32_t mask, bool addToNextScene);
	void setTexture(uint32_t slot, PCBaseTexture *tex, TextureFilter filter, float unknown);
	void drawRenderListsInternal();
};

CommonRenderDevice *createPCRenderDevice(HWND hwnd, uint32_t width, uint32_t height, bool unknown);

} // namespace cdc
