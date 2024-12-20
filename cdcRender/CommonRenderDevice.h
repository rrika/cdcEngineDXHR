#pragma once
#include <stdint.h>
#include "CommonScene.h"
#include "LinearAllocator.h"
#include "renderdevice.h"
#include "RenderPasses.h"
#include "Types.h"

namespace cdc {

class CommonLightManager;
class CommonScene;
class IMaterial;
class IMatrixState;
class IRenderPassCallback;
class IRenderTerrain;
class IRenderTerrainInstance;
class IShaderLib;
class PrimitiveContext;
class RenderExternalResource;
class RenderMesh;
class RenderModelInstance;
class RenderResource;
class TextureMap;
enum AllocRequester : uint32_t;
struct RenderVertex;

class CommonRenderDevice
{
public:
	LinearAllocator linear4; // 4
	LinearAllocator *linear30; // 30
	LinearAllocator *linear34; // 34
	float m_frameTimeDelta; // 44
	uint32_t m_currentTime; // 48
	uint32_t m_sceneCreationCount; // 54
	uint32_t m_frameIndex; // 58
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
	virtual void resetRenderLists(float timeDelta) = 0;
	virtual void drawRenderLists() = 0; // 10
	virtual void method_14();
	virtual void method_18();
	virtual bool beginRenderList(float*) = 0; // 1C, BeginSubFrame
	virtual bool endRenderList() = 0; // 20, EndSubFrame
	virtual bool hasRenderList() = 0; // 24, IsInSubFrame
	virtual uint32_t getSubFrameWidth() = 0; // 28
	virtual uint32_t getSubFrameHeight() = 0; // 2C
	virtual void method_30() = 0;
	virtual void method_34();
	virtual void method_38();
	virtual uint32_t getFrameIndex(); // 3C, see also getSceneCreationCount
	virtual void method_40();
	virtual void method_44();
	virtual CommonScene *createSubScene( // BeginScene
		RenderViewport *renderViewport,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		CommonRenderTarget *sourceColor,
		CommonDepthBuffer *sourceDepth) = 0;
	virtual void createSiblingScene( // CreatePartition
		RenderViewport *renderViewport,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		CommonRenderTarget *sourceColor,
		CommonDepthBuffer *sourceDepth);
	virtual void finishScene() = 0; // 50, EndScene
	virtual void method_54();
	virtual uint32_t getSceneCreationCount(); // 58, see also getFrameIndex
	virtual CommonScene *getScene();
	virtual CommonRenderTarget *getSceneRenderTarget() = 0; // 60
	virtual CommonDepthBuffer *getSceneDepthBuffer() = 0;
	virtual RenderViewport *getCurViewport();
	virtual uint32_t getContextWidth() = 0;
	virtual uint32_t getContextHeight() = 0;

	virtual uint32_t addPass(RenderPassType passType, uint32_t order, uint32_t sortMode, uint32_t funcSetIndex);
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
	virtual void DrawPrimitive(Matrix*, TextureMap*, RenderVertex*, uint32_t, uint32_t, float) = 0;
	virtual void DrawIndexedPrimitive(PrimitiveContext*, uint32_t, uint32_t, float) = 0;
	virtual void DrawIndexedPrimitive(PrimitiveContext*, void*, VertexDecl*, uint32_t, uint16_t*, uint32_t, float) = 0;
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
	virtual void SetGlobalParams(uint32_t index, uint32_t count, float *params);
	virtual void method_118();
	virtual void method_11C();
	virtual void *AllocateMemory(uint32_t size, uint32_t align, AllocRequester *requester); // 120
	virtual void FreeMemory(void *ptr);
	virtual void *linearAlloc30(uint32_t size, uint32_t requester); // 128
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
	virtual IRenderTarget *createRenderTarget(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, TextureClass) = 0;
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
	virtual void *InternalAlloc(uint32_t); // 198
	virtual void InternalFree(void*); // 19C
	virtual void method_1A0();

	// our own additions
	virtual void *captureRenderLists() = 0;
	virtual void revisitRenderLists(void*) = 0;
	virtual void freeRenderLists(void*) = 0;

	void DrawIndexedPrimitive(
		Matrix     *toWorld,
		void       *verts,
		VertexDecl *vertexDecl,
		uint32_t    numVerts,
		uint16_t   *indexBuffer,
		uint32_t    numPrims,
		uint32_t    primFlags,
		IMaterial  *material,
		MaterialInstanceParams *mip,
		float       sortZ,
		uint32_t    passMask,
		Matrix     *projectOverride);
	static RenderResource *createResource(uint32_t, uint32_t);
	LinearAllocator *getLinear() { return linear30; }
	void freeTemporarySurfaces();
	void DeferredRelease(RenderExternalResource*);
};

extern CommonRenderDevice *g_renderDevice;

} // namespace cdc

inline void *operator new(size_t count, cdc::CommonRenderDevice *renderDevice, uint32_t requester=0) {
	return renderDevice->linearAlloc30(uint32_t(count), requester);
}

inline void *operator new[](size_t count, cdc::CommonRenderDevice *renderDevice, uint32_t requester=0) {
	return renderDevice->linearAlloc30(uint32_t(count), requester);
}
