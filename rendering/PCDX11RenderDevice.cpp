#include "BuiltinResources.h"
#include "PCDX11ClearDrawable.h"
#include "PCDX11DepthBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderModel.h"
#include "PCDX11RenderModelInstance.h"
#include "PCDX11RenderPassCallbacks.h"
#include "PCDX11RenderTarget.h"
#include "PCDX11Scene.h"
#include "PCDX11ShaderLib.h"
#include "PCDX11StateManager.h"
#include "PCDX11Texture.h"
#include "RenderPasses.h"

namespace cdc {

PCDX11RenderDevice::RenderList::RenderList(PCDX11RenderDevice *renderDevice, void *dimensions) :
	drawableList { renderDevice->getRingBuffer() },
	next (nullptr)
{
	// TODO
	(void)dimensions;
}

PCDX11RenderDevice::PCDX11RenderDevice() :
	shtab_vs_wvp_1_0(shad::shader_30_vs, /*takeCopy=*/ false, /*wineWorkaround=*/ true),
	shtab_vs_ui(shad::shader_29_vs, /*takeCopy=*/ false),
	shtab_vs_wvp(shad::shader_28_vs, /*takeCopy=*/ false, /*wineWorkaround=*/ true),
	shtab_ps_white_27(shad::white_27_ps, /*takeCopy=*/ false),
	shtab_ps_passthrough(shad::passthrough_ps, /*takeCopy=*/ false),
	shtab_ps_white_25(shad::white_25_ps, /*takeCopy=*/ false),
	shtab_ps_fogColor(shad::fogColor_ps, /*takeCopy=*/ false),
	shtab_ps_errorColor(shad::errorColor_ps, /*takeCopy=*/ false)
{
	d3dDeviceContext111580 = deviceManager->getD3DDeviceContext();
	createDefaultResources();
	createDefaultVertexAttribLayouts();

	renderPasses.addRenderPass(0, 0x1000, 1,  1, 0);  // Depth0
	renderPasses.addRenderPass(0, 0x2000, 1,  4, 1);  // Composite
	renderPasses.addRenderPass(0, 0x3000, 1,  3, 2);  // Opaque
	renderPasses.addRenderPass(0, 0x4000, 0,  5, 3);  // Translucent
	renderPasses.addRenderPass(0, 0x8000, 0,  5, 4);  // Fullscreen
	renderPasses.addRenderPass(0, 0x9000, 0,  5, 5);  // PostFx
	renderPasses.addRenderPass(0, 0x5000, 0,  7, 6);  // AlphaBloomFsx
	renderPasses.addRenderPass(0, 0x7000, 0,  6, 7);  // Predator
	renderPasses.addRenderPass(0, 0x6000, 1,  8, 8);
	renderPasses.addRenderPass(2, 0x8000, 1,  2, 9);
	renderPasses.addRenderPass(0, 0x1850, 2,  0, 10); // DepthDependent
	renderPasses.addRenderPass(0, 0x1800, 1, 10, 12); // Normal
	renderPasses.addRenderPass(0, 0x1900, 2,  5, 13); // DeferredShading
	renderPasses.addRenderPass(0, 0x1400, 1,  1, 14); // Depth14
	setupPassCallbacks();
	registerComparatorsAndDrawersModel();
	registerComparatorsAndDrawersTerrain1();
	registerComparatorsAndDrawersTerrain2();
	internalCreateIfDeviceManagerAgrees();
}

void PCDX11RenderDevice::createDefaultResources() {
	// TODO
	shlib_22 = new PCDX11ShaderLib(shad::shader_22_ps, PCDX11ShaderLib::kPixel, this);
	shlib_21 = new PCDX11ShaderLib(shad::shader_21_vs, PCDX11ShaderLib::kVertex, this);
	shlib_20 = new PCDX11ShaderLib(shad::shader_20_vs, PCDX11ShaderLib::kVertex, this);
	shlib_19 = new PCDX11ShaderLib(shad::aa_19_vs, PCDX11ShaderLib::kVertex, this);
	shlib_18 = new PCDX11ShaderLib(shad::copy_18_vs, PCDX11ShaderLib::kVertex, this);
	shlib_17 = new PCDX11ShaderLib(shad::copy_17_ps, PCDX11ShaderLib::kPixel, this);
	shlib_16 = new PCDX11ShaderLib(shad::normalPass_16_ps, PCDX11ShaderLib::kPixel, this);
	shlib_15 = new PCDX11ShaderLib(shad::normalPass_15_vs, PCDX11ShaderLib::kVertex, this);
	shlib_14 = new PCDX11ShaderLib(shad::normalPass_14_vs, PCDX11ShaderLib::kVertex, this);
	shlib_13 = new PCDX11ShaderLib(shad::mlaa_13_ps, PCDX11ShaderLib::kPixel, this);
	shlib_12 = new PCDX11ShaderLib(shad::fxaa_12_ps, PCDX11ShaderLib::kPixel, this);
	shlib_11 = new PCDX11ShaderLib(shad::mlaa_11_ps, PCDX11ShaderLib::kPixel, this);
	shlib_10 = new PCDX11ShaderLib(shad::mlaa_10_ps, PCDX11ShaderLib::kPixel, this);
	shlib_9 = new PCDX11ShaderLib(shad::shader_9_ps, PCDX11ShaderLib::kPixel, this);
	shlib_8 = new PCDX11ShaderLib(shad::shader_8_ps, PCDX11ShaderLib::kPixel, this);
	shlib_7 = new PCDX11ShaderLib(shad::shader_7_vs, PCDX11ShaderLib::kVertex, this, /*wineWorkaround=*/ true);
	shlib_6 = new PCDX11ShaderLib(shad::shader_6_vs, PCDX11ShaderLib::kVertex, this, /*wineWorkaround=*/ true);
	shlib_5 = new PCDX11ShaderLib(shad::shader_5_ps, PCDX11ShaderLib::kPixel, this);
	shlib_4 = new PCDX11ShaderLib(shad::fastBlur0_cs, PCDX11ShaderLib::kCompute, this);
	shlib_3 = new PCDX11ShaderLib(shad::fastBlur1_cs, PCDX11ShaderLib::kCompute, this);
	shlib_2 = new PCDX11ShaderLib(shad::bilateralBlur_cs, PCDX11ShaderLib::kCompute, this);
	shlib_1 = new PCDX11ShaderLib(shad::error_ps, PCDX11ShaderLib::kPixel, this);
	shlib_0 = new PCDX11ShaderLib(shad::error_vs, PCDX11ShaderLib::kVertex, this);
	// TODO
}

void PCDX11RenderDevice::createDefaultVertexAttribLayouts() {
	// TODO
}

void PCDX11RenderDevice::setupPassCallbacks() {
	setPassCallback( 0, &depthPassCallbacks);
	setPassCallback(14, &depthPassCallbacks);
	setPassCallback(10, &depthDependentPassCallbacks);
	setPassCallback( 1, &compositePassCallbacks);
	setPassCallback( 2, &opaquePassCallbacks);
	setPassCallback( 3, &translucentPassCallbacks);
	setPassCallback( 6, &alphaBloomFSXPassCallbacks);
	setPassCallback( 7, &predatorPassCallbacks);
	setPassCallback( 4, &fullScreenFXPassCallbacks);
	setPassCallback( 5, &postFSXPassCallbacks);
	setPassCallback(12, &normalPassCallbacks);
	setPassCallback(13, &deferredShadingPassCallbacks);
}

void PCDX11RenderDevice::registerComparatorsAndDrawersModel() {
	RenderFunc todoCmp128 = nullptr;
	RenderFunc todoCmp7 = nullptr;
	RenderFunc todoCmp46 = nullptr;
	RenderFunc todoCmpA = nullptr;

	RenderFunc todoDraw1 = nullptr;
	RenderFunc todoDraw2 = nullptr;
	RenderFunc todoDraw7 = nullptr;
	RenderFunc todoDraw4 = nullptr;
	RenderFunc todoDraw56 = nullptr;
	RenderFunc todoDrawA = nullptr;

	allocFuncIndex("RenderModelDrawable");
	uint8_t funcIndex = 1;
	registerComparator(funcIndex, 1, todoCmp128);
	registerComparator(funcIndex, 2, todoCmp128);
	registerComparator(funcIndex, 8, todoCmp128);
	registerComparator(funcIndex, 7, todoCmp7);
	registerComparator(funcIndex, 4, todoCmp46);
	registerComparator(funcIndex, 6, todoCmp46);

	registerDrawer(funcIndex, 1, todoDraw1);
	registerDrawer(funcIndex, 2, todoDraw2);
	registerDrawer(funcIndex, 7, todoDraw7);
	registerDrawer(funcIndex, 4, todoDraw4);
	registerDrawer(funcIndex, 5, todoDraw56);
	registerDrawer(funcIndex, 6, todoDraw56);

	registerComparator(funcIndex, 10, todoCmpA);
	registerDrawer(funcIndex, 10, todoDrawA);
}

void PCDX11RenderDevice::registerComparatorsAndDrawersTerrain1() {
	RenderFunc todoCmp12 = nullptr;
	RenderFunc todoCmp467 = nullptr;
	RenderFunc todoCmpA = nullptr;

	RenderFunc todoDraw1 = nullptr;
	RenderFunc todoDraw2 = nullptr;
	RenderFunc todoDraw7 = nullptr;
	RenderFunc todoDraw4 = nullptr;
	RenderFunc todoDraw56 = nullptr;
	RenderFunc todoDrawA = nullptr;

	uint8_t funcIndex = allocFuncIndex("RenderTerrainDrawable");
	registerComparator(funcIndex, 1, todoCmp12);
	registerComparator(funcIndex, 2, todoCmp12);
	registerComparator(funcIndex, 7, todoCmp467);
	registerComparator(funcIndex, 4, todoCmp467);
	registerComparator(funcIndex, 6, todoCmp467);

	registerDrawer(funcIndex, 1, todoDraw1);
	registerDrawer(funcIndex, 2, todoDraw2);
	registerDrawer(funcIndex, 7, todoDraw7);
	registerDrawer(funcIndex, 4, todoDraw4);
	registerDrawer(funcIndex, 5, todoDraw56);
	registerDrawer(funcIndex, 6, todoDraw56);

	registerComparator(funcIndex, 10, todoCmpA);
	registerDrawer(funcIndex, 10, todoDrawA);
}

void PCDX11RenderDevice::registerComparatorsAndDrawersTerrain2() {
	RenderFunc todoCmp = nullptr;

	RenderFunc todoDraw1 = nullptr;
	RenderFunc todoDraw2 = nullptr;
	RenderFunc todoDraw7 = nullptr;
	RenderFunc todoDraw4 = nullptr;
	RenderFunc todoDraw56 = nullptr;
	RenderFunc todoDrawA = nullptr;

	uint8_t funcIndex = allocFuncIndex("RenderTerrainDrawable");
	registerComparator(funcIndex, 1, todoCmp);
	registerComparator(funcIndex, 2, todoCmp);
	registerComparator(funcIndex, 8, todoCmp);
	registerComparator(funcIndex, 7, todoCmp);
	registerComparator(funcIndex, 4, todoCmp);
	registerComparator(funcIndex, 6, todoCmp);

	registerDrawer(funcIndex, 1, todoDraw1);
	registerDrawer(funcIndex, 2, todoDraw2);
	registerDrawer(funcIndex, 7, todoDraw7);
	registerDrawer(funcIndex, 4, todoDraw4);
	registerDrawer(funcIndex, 5, todoDraw56);
	registerDrawer(funcIndex, 6, todoDraw56);

	registerComparator(funcIndex, 10, todoCmp);
	registerDrawer(funcIndex, 10, todoDrawA);
}

void PCDX11RenderDevice::refCountDec() {
	// TODO
}

void PCDX11RenderDevice::method_08() {
	// TODO
}

void PCDX11RenderDevice::resetRenderLists() {
	// TODO
	renderList_current = nullptr;
	renderList_last = nullptr;
	renderList_first = nullptr;
	// TODO
}

void PCDX11RenderDevice::drawRenderLists() {
	if (hasRenderList())
		endRenderList();
	// TODO
	drawRenderListsInternal(/*TODO*/(void*)this);
}

bool PCDX11RenderDevice::beginRenderList() {
	renderList_current = new RenderList(this, /*TODO*/nullptr);
	// TODO
	return true;
}

bool PCDX11RenderDevice::endRenderList() {
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

bool PCDX11RenderDevice::hasRenderList() {
	return renderList_current != nullptr;
}

void PCDX11RenderDevice::method_28() {
	// TODO
}

void PCDX11RenderDevice::method_2C() {
	// TODO
}

void PCDX11RenderDevice::method_30() {
	// TODO
}

CommonScene *PCDX11RenderDevice::createSubScene(
	CommonSceneSub10 *sub10,
	CommonRenderTarget *renderTarget,
	CommonDepthBuffer *depthBuffer
) {
	// TODO
	auto scene = new PCDX11Scene(
		this,
		scene7C, // parent scene perhaps?
		sub10,
		renderTarget,
		depthBuffer,
		getSceneSub114(),
		&renderPasses);
	scene7C = scene;
	return scene;
}

void PCDX11RenderDevice::finishScene() {
	// TODO
	auto *thisScene = scene7C;

	// first in list in charge of finish the siblings
	if (auto sibling = thisScene->nextScene; sibling && !thisScene->prevScene) {
		for (; sibling; sibling = sibling->nextScene) {
			scene7C = sibling;
			finishScene();
		}
		// return to current scene
		scene7C = thisScene;
	}
	// TODO
	static_cast<PCDX11Scene*>(scene7C)->addToDrawableList(&renderList_current->drawableList);
	scene7C = scene7C->parentScene;
}

void PCDX11RenderDevice::getSceneRenderTarget() {
	// TODO
}

void PCDX11RenderDevice::method_64() {
	// TODO
}

void PCDX11RenderDevice::method_6C() {
	// TODO
}

void PCDX11RenderDevice::method_70() {
	// TODO
}

void PCDX11RenderDevice::method_A8() {
	// TODO
}

void PCDX11RenderDevice::clearRenderTarget(
	uint32_t flags,
	uint32_t passMask,
	float sortOrder,
	float *clearColor,
	float clearDepth,
	uint32_t clearStencil)
{
	// TODO allocation
	uint32_t clearColorI =
		((int)(clearColor[3] * 255.0) << 24) |
		((int)(clearColor[0] * 255.0) << 16) |
		((int)(clearColor[1] * 255.0) <<  8) |
		((int)(clearColor[2] * 255.0) <<  0);
	(void)sortOrder; // assign to IRenderDrawable::float8
	auto clearDrawable = new PCDX11ClearDrawable(
		this,
		(flags & 2 | (flags >> 1) & 0xC) >> 1,
		clearColorI,
		clearDepth,
		clearStencil);
	recordDrawable(clearDrawable, passMask, 0);
	// TODO: don't leak PCDX11ClearDrawable instances
}

void PCDX11RenderDevice::setRenderTarget() {
	// TODO
}

void PCDX11RenderDevice::method_C0() {
	// TODO
}

void PCDX11RenderDevice::method_C4() {
	// TODO
}

void PCDX11RenderDevice::method_C8() {
	// TODO
}

void PCDX11RenderDevice::method_CC() {
	// TODO
}

void PCDX11RenderDevice::method_D0() {
	// TODO
}

void PCDX11RenderDevice::method_D4() {
	// TODO
}

void PCDX11RenderDevice::method_D8() {
	// TODO
}

void PCDX11RenderDevice::method_DC() {
	// TODO
}

void PCDX11RenderDevice::method_EC() {
	// TODO
}

void PCDX11RenderDevice::createMaterial() {
	// TODO
}

TextureMap *PCDX11RenderDevice::createTexture(uint32_t) {
	return new PCDX11Texture(this);
}

void PCDX11RenderDevice::createProceduralTexture() {
	// TODO
}

IShaderLib *PCDX11RenderDevice::createShaderLib(uint32_t size) {
	return new PCDX11ShaderLib(size, this);
}

RenderResource *PCDX11RenderDevice::createRenderModel(uint32_t size) {
	return new PCDX11RenderModel(this, size);
}

RenderModelInstance *PCDX11RenderDevice::createRenderModelInstance(RenderMesh *renderMesh) {
	return new PCDX11RenderModelInstance(static_cast<PCDX11RenderModel*>(renderMesh), this);
}

RenderResource *PCDX11RenderDevice::createRenderTerrain(uint32_t) {
	// TODO
	return nullptr;
}

void PCDX11RenderDevice::createRenderTerrainInstance() {
	// TODO
}

void PCDX11RenderDevice::createRenderImage() {
	// TODO
}

void PCDX11RenderDevice::createWaterSurface() {
	// TODO
}

void PCDX11RenderDevice::createLightResource() {
	// TODO
}

void PCDX11RenderDevice::createRenderLight() {
	// TODO
}

void PCDX11RenderDevice::createMatrixState() {
	// TODO
}

void PCDX11RenderDevice::createVertexBuffer() {
	// TODO
}

void PCDX11RenderDevice::createIndexBuffer() {
	// TODO
}

void PCDX11RenderDevice::method_16C() {
	// TODO
}

void PCDX11RenderDevice::createDepthBuffer() {
	// TODO
}

void PCDX11RenderDevice::method_174() {
	// TODO
}

void PCDX11RenderDevice::getWindowSize(uint *width, uint *height) {
	// TODO
}

void PCDX11RenderDevice::method_17C() {
	// TODO
}

void PCDX11RenderDevice::method_180() {
	// TODO
}

bool PCDX11RenderDevice::internalCreate() {
	// TODO
	return true;
}

void PCDX11RenderDevice::internalRelease() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_0() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_4() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_8() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_C() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_10() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_14() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_18() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_1C() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_20() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_24() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_28() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_2C() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_30() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_34() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_38() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_3C() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_40() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_44() {
	// TODO
}

void PCDX11RenderDevice::dx11_method_48() {
	// TODO
}

void PCDX11RenderDevice::recordDrawable(IRenderDrawable *drawable, uint32_t mask, bool addToNextScene) {
	// TODO
	// drawable->draw(0, nullptr); // hack
	scene7C->drawableListsAndMasks->add(drawable, mask);
}

void PCDX11RenderDevice::clearRenderTargetNow(char flags, float *color, float depth, uint32_t stencil) {
	auto deviceContext = d3dDeviceContext111580;
	auto stateManager = deviceManager->getStateManager();
	if (auto depthBuffer = stateManager->m_depthBuffer) {
		ID3D11DepthStencilView *view = depthBuffer->getDepthStencilView();
		deviceContext->ClearDepthStencilView(view, (flags >> 1) & 3, depth, stencil);
	}
	if (auto renderTarget = stateManager->m_renderTarget) {
		ID3D11RenderTargetView *view = renderTarget->getRenderTargetView();
		if (flags & 1)
			deviceContext->ClearRenderTargetView(view, color);
	}
}

void PCDX11RenderDevice::setTexture(uint32_t slot, PCDX11BaseTexture *tex, uint32_t filter, float unknown) {
	if (!tex)
		tex = missingTexture;
	deviceManager->getStateManager()->setTextureAndSampler(slot, tex, filter, unknown);
}

void PCDX11RenderDevice::drawRenderListsInternal(void *arg) {
	// TODO
	renderList_processing = renderList_first;
	// TODO
	if (arg) {
		// TODO
		while (renderList_processing) {
			// TODO
			renderList_processing->drawableList.draw(renderPasses.drawers, /*funcSetIndex=*/0);
			renderList_processing = renderList_processing->next;
		}
	}
}

PCDX11RenderDevice *createPCDX11RenderDevice(HWND hwnd, uint width, uint height, bool unknown) {
	// createPCDX11DeviceManager(); // already done, else wouldn't have an hwnd
	return new PCDX11RenderDevice();
}

}
