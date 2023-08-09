#include "PCMaterial.h"
#include "PCModelDrawable.h"
#include "PCRenderDevice.h"
#include "PCRenderModel.h"
#include "PCScene.h"
#include "shaders/PCShaderLib.h"
#include "surfaces/PCTexture.h"

namespace cdc {

PCRenderDevice::PCRenderDevice(HWND hwnd, uint32_t width, uint32_t height) {
	// TODO
	renderContext = CreateRenderContext(hwnd, width, height, /*useMultiSapmle=*/ true);
	// TODO
	renderPasses.addRenderPass(kRegularPass, 0x1800, 1, /*10*/ kRenderFunctionNormal, /*12*/ kPassIndexNormal);
	registerComparatorsAndDrawersModel();
}

void PCRenderDevice::registerComparatorsAndDrawersModel() {
	allocFuncIndex("RenderModelDrawable");
	uint8_t funcIndex = 1;
	// registerComparator(funcIndex, /*1*/ kRenderFunctionDepth,         PCModelDrawable::compare128);
	// registerComparator(funcIndex, /*2*/ kRenderFunctionShadow,        PCModelDrawable::compare128);
	// registerComparator(funcIndex, /*8*/ kRenderFunction8,             PCModelDrawable::compare128);
	// registerComparator(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, PCModelDrawable::compare7);
	// registerComparator(funcIndex, /*4*/ kRenderFunctionComposite,     PCModelDrawable::compare46);
	// registerComparator(funcIndex, /*6*/ kRenderFunctionPredator,      PCModelDrawable::compare46);

	// registerDrawer(funcIndex, /*1*/ kRenderFunctionDepth,         PCModelDrawable::drawDepth);
	// registerDrawer(funcIndex, /*2*/ kRenderFunctionShadow,        PCModelDrawable::drawShadow);
	// registerDrawer(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, PCModelDrawable::drawAlphaBloom);
	// registerDrawer(funcIndex, /*4*/ kRenderFunctionComposite,     PCModelDrawable::drawComposite);
	// registerDrawer(funcIndex, /*5*/ kRenderFunctionTranslucent,   PCModelDrawable::drawTranslucent);
	// registerDrawer(funcIndex, /*6*/ kRenderFunctionPredator,      PCModelDrawable::drawTranslucent);

	// registerComparator(funcIndex, /*10*/ kRenderFunctionNormal, PCModelDrawable::compareNormal);
	registerDrawer(funcIndex, /*10*/ kRenderFunctionNormal, PCModelDrawable::drawNormal);
}


PCRenderContext	*PCRenderDevice::CreateRenderContext(HWND hwnd, uint32_t width, uint32_t height, bool useMultiSample) {

	class ExternalRenderContext : public PCRenderContext, RenderExternalResource {
	public:
		ExternalRenderContext(HWND hwnd, uint32_t width, uint32_t height, bool useMultiSample, PCRenderDevice *renderDevice) :
			PCRenderContext(hwnd, width, height, useMultiSample, renderDevice),
			RenderExternalResource()
		{}
	};

	auto externalRenderContext = new ExternalRenderContext(hwnd, width, height, useMultiSample, this);
	return static_cast<PCRenderContext*>(externalRenderContext);
}

void PCRenderDevice::refCountDec() {
	// TODO
}

void PCRenderDevice::method_08() {
	// TODO
}

void PCRenderDevice::resetRenderLists() {
	// copied from PCDX11RenderDevice, need proper implementation

	// TODO
	renderList_current = nullptr;
	renderList_last = nullptr;
	renderList_first = nullptr;
	// TODO
}

void PCRenderDevice::drawRenderLists() {
	// copied from PCDX11RenderDevice, need proper implementation

	if (hasRenderList())
		endRenderList();
	// TODO
	drawRenderListsInternal();
}

bool PCRenderDevice::beginRenderList(float*) {
	// copied from PCDX11RenderDevice, need proper implementation

	renderList_current = new (linear30, 0, true) RenderList(this);
	return true;
}

bool PCRenderDevice::endRenderList() {
	// copied from PCDX11RenderDevice, need proper implementation

	if (renderList_last) {
		renderList_last->next = renderList_current;
	} else {
		renderList_first = renderList_current;
	}
	renderList_last = renderList_current;
	renderList_current = 0;
	// TODO
	return true;
}

bool PCRenderDevice::hasRenderList() {
	// TODO
	return true;
}

uint32_t PCRenderDevice::getSubFrameWidth() {
	// TODO
	return 0;
}

uint32_t PCRenderDevice::getSubFrameHeight() {
	// TODO
	return 0;
}

void PCRenderDevice::method_30() {
	// TODO
}

CommonScene *PCRenderDevice::createSubScene(
	RenderViewport *viewport,
	CommonRenderTarget *renderTarget,
	CommonDepthBuffer *depthBuffer)
{
	// copied from PCDX11RenderDevice, need proper implementation

	// TODO
	auto scene = new (linear30, 1, true) PCScene(
		this,
		scene7C, // parent scene perhaps?
		viewport,
		renderTarget,
		depthBuffer,
		getGlobalState(),
		&renderPasses);
	scene7C = scene;
	return scene;
}

void PCRenderDevice::finishScene() {
	// copied from PCDX11RenderDevice, need proper implementation

	// TODO
	// static_cast<PCScene*>(scene7C)->addToDrawableList(&renderList_current->drawableList);
	renderList_current->drawableList.add(scene7C); // HACK
	scene7C = scene7C->parentScene;
}

void PCRenderDevice::getSceneRenderTarget() {
	// TODO
}

void PCRenderDevice::method_64() {
	// TODO
}

void PCRenderDevice::method_6C() {
	// TODO
}

void PCRenderDevice::method_70() {
	// TODO
}

void PCRenderDevice::method_A8() {
	// TODO
}

void PCRenderDevice::clearRenderTarget(
	uint32_t flags,
	uint32_t unknown1,
	float unknown2,
	float *clearColor,
	float clearDepth,
	uint32_t clearStencil)
{
	// TODO
}

void PCRenderDevice::setRenderTarget() {
	// TODO
}

void PCRenderDevice::method_C0() {
	// TODO
}

void PCRenderDevice::method_C4() {
	// TODO
}

void PCRenderDevice::method_C8() {
	// TODO
}

void PCRenderDevice::method_CC() {
	// TODO
}

void PCRenderDevice::method_D0() {
	// TODO
}

void PCRenderDevice::method_D4() {
	// TODO
}

void PCRenderDevice::method_D8() {
	// TODO
}

void PCRenderDevice::method_DC() {
	// TODO
}

void PCRenderDevice::method_EC() {
	// TODO
}

void PCRenderDevice::method_F4() {
	// TODO
}

void PCRenderDevice::method_108() {
	// TODO
}

IMaterial *PCRenderDevice::createMaterial() {
	return new PCMaterial(this);
}

TextureMap *PCRenderDevice::createTexture(uint32_t) {
	return new PCTexture(this);
}

void PCRenderDevice::createProceduralTexture() {
	// TODO
}

IShaderLib *PCRenderDevice::createShaderLib(uint32_t size) {
	return new PCShaderLib(size, this);
}

RenderMesh *PCRenderDevice::createRenderModel(uint32_t size) {
	return new PCRenderModel(size, this);
}

RenderModelInstance *PCRenderDevice::createRenderModelInstance(RenderMesh*) {
	// TODO
	return nullptr;
}

IRenderTerrain *PCRenderDevice::createRenderTerrain(uint32_t) {
	// TODO
	return nullptr;
}

IRenderTerrainInstance *PCRenderDevice::createRenderTerrainInstance(IRenderTerrain*) {
	// TODO
	return nullptr;
}

void PCRenderDevice::createRenderImage() {
	// TODO
}

void PCRenderDevice::createWaterSurface() {
	// TODO
}

void PCRenderDevice::createLightResource() {
	// TODO
}

void PCRenderDevice::createRenderLight() {
	// TODO
}

IMatrixState *PCRenderDevice::createMatrixState() {
	// TODO
	return nullptr;
}

void PCRenderDevice::createVertexBuffer() {
	// TODO
}

void PCRenderDevice::createIndexBuffer() {
	// TODO
}

IRenderTarget *PCRenderDevice::createRenderTarget(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) {
	// TODO
	return nullptr;
}

IDepthBuffer *PCRenderDevice::createDepthBuffer() {
	// TODO
	return nullptr;
}

void PCRenderDevice::method_174() {
	// TODO
}

void PCRenderDevice::getWindowSize(uint32_t *width, uint32_t *height) {
	// TODO
}

void PCRenderDevice::method_17C() {
	// TODO
}

void PCRenderDevice::method_180() {
	// TODO
}

void PCRenderDevice::method_184() {
	// TODO
}

void PCRenderDevice::method_198() {
	// TODO
}

void PCRenderDevice::method_19C() {
	// TODO
}

void *PCRenderDevice::captureRenderLists() {
	// TODO
	return nullptr;
}

void PCRenderDevice::revisitRenderLists(void*) {
	// TODO
}

void PCRenderDevice::freeRenderLists(void*) {
	// TODO
}

bool PCRenderDevice::internalCreate() {
	// TODO
	return true;
}

void PCRenderDevice::internalRelease() {
	// TODO
}

PCRenderContext *PCRenderDevice::getRenderContext() {
	return renderContext; // 10C9C
}

void PCRenderDevice::recordDrawable(IRenderDrawable *drawable, uint32_t mask, bool addToNextScene) {
	// copied from PCDX11RenderDevice, need proper implementation

	// TODO
	// drawable->draw(0, nullptr); // hack
	scene7C->drawableListsAndMasks->add(drawable, mask);
}

void PCRenderDevice::drawRenderListsInternal() {
	// copied from PCDX11RenderDevice, need proper implementation

	// TODO
	auto linear = linear34;
	linear34 = linear30;
	linear30 = linear;
	linear->rewind();

	// TODO
	if (renderList_override)
		renderList_processing = renderList_override;
	else
		renderList_processing = renderList_first;

	// TODO
	if (true) {
		// TODO
		while (renderList_processing) {
			// TODO
			renderList_processing->drawableList.draw(renderPasses.drawers, /*funcSetIndex=*/0);
			renderList_processing = renderList_processing->next;
		}
	}

	freeTemporarySurfaces();
}

CommonRenderDevice *createPCRenderDevice(HWND hwnd, uint32_t width, uint32_t height, bool unknown) {
	// HACK
	g_renderDevice = new PCRenderDevice(hwnd, width, height/*, unknown*/);
	return g_renderDevice;
}

}
