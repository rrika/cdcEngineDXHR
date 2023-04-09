#pragma once
#include <stdint.h>
#include "CommonScene.h"
#include "LinearAllocator.h"
#include "RenderPasses.h"

namespace cdc {

class CommonLightManager;
class CommonScene;
class IMaterial;
class IMatrixState;
class IRenderPassCallback;
class IRenderTerrain;
class IRenderTerrainInstance;
class IShaderLib;
class RenderMesh;
class RenderModelInstance;
class RenderResource;
class TextureMap;

class CommonRenderDevice
{
public:
	LinearAllocator linear4; // 4
	LinearAllocator *linear30; // 30
	LinearAllocator *linear34; // 34
	uint32_t sceneCreationCount; // 54
public:
	CommonScene *scene78; // 78
protected:
	CommonScene *scene7C = nullptr; // 7C
public:
	CommonLightManager *lightManager; // 84
protected:
	RenderGlobalState globalState_8C; // 8C
public:

	IRenderSurface *temporarySurfaces[64]; // 1E4
	uint32_t numTemporarySurfaces = 0; // 2E4

	RenderPasses renderPasses; // 2E8
public:
	CommonRenderDevice();

	virtual ~CommonRenderDevice() = default;
	virtual void refCountDec() = 0;
	virtual void method_08() = 0;
	virtual void resetRenderLists() = 0;
	virtual void drawRenderLists() = 0; // 10
	virtual void method_14();
	virtual void method_18();
	virtual bool beginRenderList(float*) = 0;
	virtual bool endRenderList() = 0; // 20
	virtual bool hasRenderList() = 0;
	virtual uint32_t getSubFrameWidth() = 0;
	virtual uint32_t getSubFrameHeight() = 0;
	virtual void method_30() = 0;
	virtual void method_34();
	virtual void method_38();
	virtual void method_3C();
	virtual void method_40();
	virtual void method_44();
	virtual CommonScene *createSubScene(
		RenderViewport *renderViewport,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer) = 0;
	virtual void createSiblingScene(
		RenderViewport *renderViewport,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer);
	virtual void finishScene() = 0; // 50
	virtual void method_54();
	virtual uint32_t getSceneCreationCount(); // frame count, basically
	virtual CommonScene *getScene();
	virtual void getSceneRenderTarget() = 0; // 60
	virtual void method_64() = 0;
	virtual RenderViewport *getCurViewport();
	virtual void method_6C() = 0;
	virtual void method_70() = 0;

	virtual void addPass(RenderPassType passType, uint32_t order, uint32_t sortMode, uint32_t funcSetIndex);
	virtual void method_78();
	virtual uint32_t getPassOrder(uint32_t);
	virtual void method_80();
	virtual void setPassCallback(uint32_t passId, IRenderPassCallback *cb);
	virtual void method_88();
	virtual void method_8C();

	virtual uint8_t allocFuncIndex(const char *name);
	virtual void freeFuncIndex(uint8_t);
	virtual void registerDrawer(uint32_t funcIndex, uint32_t funcSetIndex, DrawableRenderFn drawer);
	virtual DrawableRenderFn getDrawer(uint32_t funcIndex, uint32_t funcSetIndex);
	virtual void registerComparator(uint32_t funcIndex, uint32_t funcSetIndex, DrawableCompareFn comparator);
	virtual DrawableCompareFn getComparator(uint32_t funcIndex, uint32_t funcSetIndex);

	virtual void method_A8() = 0;
	virtual void method_AC();
	virtual void method_B0();
	virtual void method_B4();
	virtual void clearRenderTarget(
		uint32_t flags,
		uint32_t unknown1,
		float unknown2,
		float *clearColor,
		float clearDepth,
		uint32_t clearStencil) = 0;
	virtual void setRenderTarget() = 0;
	virtual void method_C0() = 0;
	virtual void method_C4() = 0;
	virtual void method_C8() = 0;
	virtual void method_CC() = 0;
	virtual void method_D0() = 0;
	virtual void method_D4() = 0;
	virtual void method_D8() = 0;
	virtual void method_DC() = 0;
	virtual void method_E0();
	virtual void method_E4();
	virtual RenderGlobalState *getGlobalState();
	virtual void method_EC() = 0;
	virtual void method_F0();
	virtual void method_F4();
	virtual void method_F8();
	virtual void method_FC();
	virtual void method_100();
	virtual void method_104();
	virtual void method_108();
	virtual void method_10C();
	virtual void method_110();
	virtual void method_114();
	virtual void method_118();
	virtual void method_11C();
	virtual void method_120();
	virtual void method_124();
	virtual void *linearAlloc30(uint32_t, uint32_t); // 128
	virtual IMaterial *createMaterial() = 0;
	virtual TextureMap *createTexture(uint32_t) = 0;
	virtual void createProceduralTexture() = 0;
	virtual IShaderLib *createShaderLib(uint32_t) = 0;
	virtual RenderMesh *createRenderModel(uint32_t) = 0;
	virtual RenderResource *createRenderModelData(uint32_t);
	virtual RenderModelInstance *createRenderModelInstance(RenderMesh*) = 0;
	virtual IRenderTerrain *createRenderTerrain(uint32_t) = 0;
	virtual IRenderTerrainInstance *createRenderTerrainInstance(IRenderTerrain*) = 0;
	virtual void createRenderImage() = 0;
	virtual void createWaterSurface() = 0;
	virtual void createLightResource() = 0;
	virtual void createRenderLight() = 0;
	virtual IMatrixState *createMatrixState() = 0;
	virtual void createVertexBuffer() = 0;
	virtual void createIndexBuffer() = 0;
	virtual IRenderTarget *createRenderTarget(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) = 0;
	virtual IDepthBuffer *createDepthBuffer() = 0;
	virtual void method_174() = 0;
	virtual void getWindowSize(uint32_t *width, uint32_t *height) = 0;
	virtual void method_17C() = 0;
	virtual void method_180() = 0;
	virtual void method_184();
	virtual bool isInStereoMode();
	virtual void method_18C();
	virtual void method_190();
	virtual bool useAlternateLinearAlloc();
	virtual void method_198();
	virtual void method_19C();
	virtual void method_1A0();

	// our own additions
	virtual void *captureRenderLists() = 0;
	virtual void revisitRenderLists(void*) = 0;
	virtual void freeRenderLists(void*) = 0;

	static RenderResource *createResource(uint32_t, uint32_t);
	LinearAllocator *getLinear() { return linear30; }
	void freeTemporarySurfaces();
};

extern CommonRenderDevice *g_renderDevice;

} // namespace cdc

inline void *operator new(size_t count, cdc::CommonRenderDevice *renderDevice, uint32_t requester=0) {
	return renderDevice->linearAlloc30(uint32_t(count), requester);
}

inline void *operator new[](size_t count, cdc::CommonRenderDevice *renderDevice, uint32_t requester=0) {
	return renderDevice->linearAlloc30(uint32_t(count), requester);
}
