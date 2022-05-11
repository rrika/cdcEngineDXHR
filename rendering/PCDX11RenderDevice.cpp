#include "BuiltinResources.h"
#include "PCDX11ClearDrawable.h"
#include "PCDX11DepthBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderTarget.h"
#include "PCDX11Scene.h"
#include "PCDX11ShaderLib.h"
#include "PCDX11StateManager.h"
#include "PCDX11Texture.h"
#include "RenderPasses.h"

namespace cdc {


PCDX11RenderDevice::PCDX11RenderDevice() :
	shtab_vs_wvp_1_0(shad::shader_30_vs, /*takeCopy=*/ false),
	shtab_vs_ui(shad::shader_29_vs, /*takeCopy=*/ false),
	shtab_vs_wvp(shad::shader_28_vs, /*takeCopy=*/ false),
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
	shlib_18 = new PCDX11ShaderLib(shad::shader_18_vs, PCDX11ShaderLib::kVertex, this);
	shlib_17 = new PCDX11ShaderLib(shad::shader_17_ps, PCDX11ShaderLib::kPixel, this);
	shlib_16 = new PCDX11ShaderLib(shad::normalPass_16_ps, PCDX11ShaderLib::kPixel, this);
	shlib_15 = new PCDX11ShaderLib(shad::normalPass_15_vs, PCDX11ShaderLib::kVertex, this);
	shlib_14 = new PCDX11ShaderLib(shad::normalPass_14_vs, PCDX11ShaderLib::kVertex, this);
	shlib_13 = new PCDX11ShaderLib(shad::mlaa_13_ps, PCDX11ShaderLib::kPixel, this);
	shlib_12 = new PCDX11ShaderLib(shad::fxaa_12_ps, PCDX11ShaderLib::kPixel, this);
	shlib_11 = new PCDX11ShaderLib(shad::mlaa_11_ps, PCDX11ShaderLib::kPixel, this);
	shlib_10 = new PCDX11ShaderLib(shad::mlaa_10_ps, PCDX11ShaderLib::kPixel, this);
	shlib_9 = new PCDX11ShaderLib(shad::shader_9_ps, PCDX11ShaderLib::kPixel, this);
	shlib_8 = new PCDX11ShaderLib(shad::shader_8_ps, PCDX11ShaderLib::kPixel, this);
	shlib_7 = new PCDX11ShaderLib(shad::shader_7_vs, PCDX11ShaderLib::kVertex, this);
	shlib_6 = new PCDX11ShaderLib(shad::shader_6_vs, PCDX11ShaderLib::kVertex, this);
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
	IRenderPassCallback *todoDepth = nullptr;
	IRenderPassCallback *todoDepthDependent = nullptr;
	IRenderPassCallback *todoComposite = nullptr;
	IRenderPassCallback *todoOpaque = nullptr;
	IRenderPassCallback *todoTranslucent = nullptr;
	IRenderPassCallback *todoAlphaBloomFsx = nullptr;
	IRenderPassCallback *todoPredator = nullptr;
	IRenderPassCallback *todoFullscreenFx = nullptr;
	IRenderPassCallback *todoPostFx = nullptr;
	IRenderPassCallback *todoNormal = nullptr;
	IRenderPassCallback *todoDeferredShading = nullptr;

	setPassCallback( 0, todoDepth);
	setPassCallback(14, todoDepth);
	setPassCallback(10, todoDepthDependent);
	setPassCallback( 1, todoComposite);
	setPassCallback( 2, todoOpaque);
	setPassCallback( 3, todoTranslucent);
	setPassCallback( 6, todoAlphaBloomFsx);
	setPassCallback( 7, todoPredator);
	setPassCallback( 4, todoFullscreenFx);
	setPassCallback( 5, todoPostFx);
	setPassCallback(12, todoNormal);
	setPassCallback(13, todoDeferredShading);
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

	allocFuncSet("RenderModelDrawable");
	uint8_t funcSet = 1;
	registerComparator(funcSet, 1, todoCmp128);
	registerComparator(funcSet, 2, todoCmp128);
	registerComparator(funcSet, 8, todoCmp128);
	registerComparator(funcSet, 7, todoCmp7);
	registerComparator(funcSet, 4, todoCmp46);
	registerComparator(funcSet, 6, todoCmp46);

	registerDrawer(funcSet, 1, todoDraw1);
	registerDrawer(funcSet, 2, todoDraw2);
	registerDrawer(funcSet, 7, todoDraw7);
	registerDrawer(funcSet, 4, todoDraw4);
	registerDrawer(funcSet, 5, todoDraw56);
	registerDrawer(funcSet, 6, todoDraw56);

	registerComparator(funcSet, 10, todoCmpA);
	registerDrawer(funcSet, 10, todoDrawA);
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

	uint8_t funcSet = allocFuncSet("RenderTerrainDrawable");
	registerComparator(funcSet, 1, todoCmp12);
	registerComparator(funcSet, 2, todoCmp12);
	registerComparator(funcSet, 7, todoCmp467);
	registerComparator(funcSet, 4, todoCmp467);
	registerComparator(funcSet, 6, todoCmp467);

	registerDrawer(funcSet, 1, todoDraw1);
	registerDrawer(funcSet, 2, todoDraw2);
	registerDrawer(funcSet, 7, todoDraw7);
	registerDrawer(funcSet, 4, todoDraw4);
	registerDrawer(funcSet, 5, todoDraw56);
	registerDrawer(funcSet, 6, todoDraw56);

	registerComparator(funcSet, 10, todoCmpA);
	registerDrawer(funcSet, 10, todoDrawA);
}

void PCDX11RenderDevice::registerComparatorsAndDrawersTerrain2() {
	RenderFunc todoCmp = nullptr;

	RenderFunc todoDraw1 = nullptr;
	RenderFunc todoDraw2 = nullptr;
	RenderFunc todoDraw7 = nullptr;
	RenderFunc todoDraw4 = nullptr;
	RenderFunc todoDraw56 = nullptr;
	RenderFunc todoDrawA = nullptr;

	uint8_t funcSet = allocFuncSet("RenderTerrainDrawable");
	registerComparator(funcSet, 1, todoCmp);
	registerComparator(funcSet, 2, todoCmp);
	registerComparator(funcSet, 8, todoCmp);
	registerComparator(funcSet, 7, todoCmp);
	registerComparator(funcSet, 4, todoCmp);
	registerComparator(funcSet, 6, todoCmp);

	registerDrawer(funcSet, 1, todoDraw1);
	registerDrawer(funcSet, 2, todoDraw2);
	registerDrawer(funcSet, 7, todoDraw7);
	registerDrawer(funcSet, 4, todoDraw4);
	registerDrawer(funcSet, 5, todoDraw56);
	registerDrawer(funcSet, 6, todoDraw56);

	registerComparator(funcSet, 10, todoCmp);
	registerDrawer(funcSet, 10, todoDrawA);
}

void PCDX11RenderDevice::refCountDec() {
	// TODO
}

void PCDX11RenderDevice::method_08() {
	// TODO
}

void PCDX11RenderDevice::method_0C() {
	// TODO
}

void PCDX11RenderDevice::method_10() {
	// TODO
}

void PCDX11RenderDevice::method_1C() {
	// TODO
}

void PCDX11RenderDevice::method_20() {
	// TODO
}

void PCDX11RenderDevice::method_24() {
	// TODO
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

CommonScene *PCDX11RenderDevice::createScene(
	CommonSceneSub18 *sub18,
	CommonRenderTarget *renderTarget,
	CommonDepthBuffer *depthBuffer
) {
	// TODO
	auto scene = new PCDX11Scene(
		this,
		scene7C, // parent scene perhaps?
		sub18,
		renderTarget,
		depthBuffer,
		getSceneSub114(),
		&renderPasses);
	scene7C = scene;
	return scene;
}

void PCDX11RenderDevice::method_50() {
	// TODO
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

RenderResource *PCDX11RenderDevice::createRenderModel(uint32_t) {
	// TODO
	return nullptr;
}

void PCDX11RenderDevice::createRenderModelInstance() {
	// TODO
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

void PCDX11RenderDevice::recordDrawable(IRenderDrawable *drawable, uint32_t maskA, uint8_t maskB) {
	// TODO
	// drawable->renderDrawable0(); // hack
	scene7C->drawableListsAndMasks->add(drawable, maskA);
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

PCDX11RenderDevice *createPCDX11RenderDevice(HWND hwnd, uint width, uint height, bool unknown) {
	// createPCDX11DeviceManager(); // already done, else wouldn't have an hwnd
	return new PCDX11RenderDevice();
}

}
