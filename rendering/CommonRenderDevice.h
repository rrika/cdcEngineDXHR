#pragma once
#include <stdint.h>
#include "../types.h"
#include "RenderPasses.h"
#include "CommonScene.h"

namespace cdc {

class IShaderLib;
class IRenderPassCallback;
class RenderResource;
class TextureMap;
class CommonScene;

class CommonRenderDevice
{
protected:
	RingBuffer *ringBuffer30; // 30
public:
	CommonScene *scene78; // 78
protected:
	CommonScene *scene7C = nullptr; // 7C
	CommonSceneSub114 sceneSub114_8C; // 8C
public:
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
	virtual bool beginRenderList() = 0;
	virtual bool endRenderList() = 0; // 20
	virtual bool hasRenderList() = 0;
	virtual void method_28() = 0;
	virtual void method_2C() = 0;
	virtual void method_30() = 0;
	virtual void method_34();
	virtual void method_38();
	virtual void method_3C();
	virtual void method_40();
	virtual void method_44();
	virtual CommonScene *createSubScene(
		CommonSceneSub10 *sub10,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer) = 0;
	virtual void createSiblingScene(
		CommonSceneSub10 *sub10,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer);
	virtual void finishScene() = 0;
	virtual void method_54();
	virtual void method_58();
	virtual void getScene();
	virtual void getSceneRenderTarget() = 0;
	virtual void method_64() = 0;
	virtual void method_68();
	virtual void method_6C() = 0;
	virtual void method_70() = 0;

	virtual void addPass();
	virtual void method_78();
	virtual uint32_t getPassOrder(uint32_t);
	virtual void method_80();
	virtual void setPassCallback(uint32_t passId, IRenderPassCallback *cb);
	virtual void method_88();
	virtual void method_8C();

	virtual uint8_t allocFuncIndex(const char *name);
	virtual void freeFuncIndex(uint8_t);
	virtual void registerDrawer(uint32_t funcIndex, uint32_t funcSetIndex, RenderFunc drawer);
	virtual RenderFunc getDrawer(uint32_t funcIndex, uint32_t funcSetIndex);
	virtual void registerComparator(uint32_t funcIndex, uint32_t funcSetIndex, RenderFunc comparator);
	virtual RenderFunc getComparator(uint32_t funcIndex, uint32_t funcSetIndex);

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
	virtual CommonSceneSub114 *getSceneSub114();
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
	virtual void method_128();
	virtual void createMaterial() = 0;
	virtual TextureMap *createTexture(uint32_t) = 0;
	virtual void createProceduralTexture() = 0;
	virtual IShaderLib *createShaderLib(uint32_t) = 0;
	virtual RenderResource *createRenderModel(uint32_t) = 0;
	virtual RenderResource *method_140(uint32_t);
	virtual void createRenderModelInstance() = 0;
	virtual RenderResource *createRenderTerrain(uint32_t) = 0;
	virtual void createRenderTerrainInstance() = 0;
	virtual void createRenderImage() = 0;
	virtual void createWaterSurface() = 0;
	virtual void createLightResource() = 0;
	virtual void createRenderLight() = 0;
	virtual void createMatrixState() = 0;
	virtual void createVertexBuffer() = 0;
	virtual void createIndexBuffer() = 0;
	virtual void method_16C() = 0;
	virtual void createDepthBuffer() = 0;
	virtual void method_174() = 0;
	virtual void getWindowSize(uint *width, uint *height) = 0;
	virtual void method_17C() = 0;
	virtual void method_180() = 0;
	virtual void method_184();
	virtual bool isInStereoMode();
	virtual void method_18C();
	virtual void method_190();
	virtual void method_194();
	virtual void method_198();
	virtual void method_19C();
	virtual void method_1A0();

	static RenderResource *createResource(uint32_t, uint32_t);
	RingBuffer *getRingBuffer() { return ringBuffer30; }
};

extern CommonRenderDevice *gRenderDevice;

}
