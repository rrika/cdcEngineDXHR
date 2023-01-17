#pragma once
#include <windows.h>
#include "rendering/CommonRenderDevice.h"
#include "rendering/pc/PCInternalResource.h"

namespace cdc {

class PCRenderDevice :
	public PCInternalResource,
	public CommonRenderDevice
{
public:
	PCRenderDevice(HWND hwnd, uint32_t width, uint32_t height);

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
		RenderViewport *renderViewport,
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

	static RenderResource *createResource(uint32_t, uint32_t);
	LinearAllocator *getLinear() { return linear30; }
	void freeTemporarySurfaces();
};

CommonRenderDevice *createPCRenderDevice(HWND hwnd, uint32_t width, uint32_t height, bool unknown);

} // namespace cdc
