#include "BuiltinResources.h"
#include "drawables/PCDX11ClearDrawable.h"
#include "LinearAllocator.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11LightManager.h"
#include "PCDX11Material.h"
#include "PCDX11MatrixState.h"
#include "PCDX11ModelDrawable.h"
#include "PCDX11RenderContext.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderModel.h"
#include "PCDX11RenderModelInstance.h"
#include "PCDX11RenderPassCallbacks.h"
#include "PCDX11RenderTerrain.h"
#include "CommonRenderTerrainInstance.h"
#include "PCDX11Scene.h"
#include "PCDX11StateManager.h"
#include "PCDX11TerrainDrawable.h"
#include "RenderPasses.h"
#include "Types.h"
#include "buffers/PCDX11SimpleDynamicVertexBuffer.h"
#include "shaders/PCDX11ShaderLib.h"
#include "surfaces/PCDX11DefaultRenderTarget.h"
#include "surfaces/PCDX11DepthBuffer.h"
// #include "surfaces/PCDX11HeapRenderTarget.h"
#include "surfaces/PCDX11RenderTarget.h"
#include "surfaces/PCDX11SubFrameRenderTarget.h"
#include "surfaces/PCDX11Texture.h"

namespace cdc {

PCDX11RenderDevice::RenderList::RenderList(PCDX11RenderDevice *renderDevice, uint32_t *dimensions) :
	drawableList { renderDevice->getLinear() },
	next (nullptr)
{
	// TODO
	if (dimensions) {
		dword0 = dimensions[0];
		dword4 = dimensions[1];
		widthMaybe8 = dimensions[2];
		heightMaybeC = dimensions[3];

		auto *currentRenderTarget = renderDevice->getRenderContextAny()->getRenderTarget();
		renderTarget = new PCDX11SubFrameRenderTarget(
			dimensions[0],
			dimensions[1],
			dimensions[2],
			dimensions[3],
			renderDevice,
			renderTarget);

	} else {
		renderTarget = renderDevice->getRenderContextAny()->getRenderTarget();
		// dword14 = ...;
		dword0 = 0;
		dword4 = 0;
		if (!renderTarget) return; // HACK
		widthMaybe8 = renderTarget->getWidth();
		heightMaybeC = renderTarget->getHeight();
	}
}

PCDX11RenderDevice::PCDX11RenderDevice(HWND hwnd, uint32_t width, uint32_t height) :
	shtab_vs_wvp_1_0(shad::shader_30_vs, /*takeCopy=*/ false, /*wineWorkaround=*/ true),
	shtab_vs_ui(shad::shader_29_vs, /*takeCopy=*/ false),
	shtab_vs_wvp(shad::shader_28_vs, /*takeCopy=*/ false, /*wineWorkaround=*/ true),
	shtab_ps_white_27(shad::white_27_ps, /*takeCopy=*/ false),
	shtab_ps_passthrough(shad::passthrough_ps, /*takeCopy=*/ false),
	shtab_ps_white_25(shad::white_25_ps, /*takeCopy=*/ false),
	shtab_ps_fogColor(shad::fogColor_ps, /*takeCopy=*/ false),
	shtab_ps_errorColor(shad::errorColor_ps, /*takeCopy=*/ false),
	streamDeclCache(this)
{
	d3dDeviceContext111580 = deviceManager->getD3DDeviceContext();

	lightManager = new PCDX11LightManager(this);

	renderContext_10CEC = new PCDX11RenderContext(hwnd, width, height, 1, this);

	createDefaultResources();
	createDefaultVertexAttribLayouts();

	renderPasses.addRenderPass(kRegularPass, 0x1000, 1, /* 1*/ kRenderFunctionDepth,         /* 0*/ kPassIndexDepth);
	renderPasses.addRenderPass(kRegularPass, 0x2000, 1, /* 4*/ kRenderFunctionComposite,     /* 1*/ kPassIndexComposite);
	renderPasses.addRenderPass(kRegularPass, 0x3000, 1, /* 3*/ kRenderFunctionOpaque,        /* 2*/ kPassIndexOpaque);
	renderPasses.addRenderPass(kRegularPass, 0x4000, 0, /* 5*/ kRenderFunctionTranslucent,   /* 3*/ kPassIndexTranslucent);
	renderPasses.addRenderPass(kRegularPass, 0x8000, 0, /* 5*/ kRenderFunctionTranslucent,   /* 4*/ kPassIndexFullScreenFX);
	renderPasses.addRenderPass(kRegularPass, 0x9000, 0, /* 5*/ kRenderFunctionTranslucent,   /* 5*/ kPassIndexPostFSX);
	renderPasses.addRenderPass(kRegularPass, 0x5000, 0, /* 7*/ kRenderFunctionAlphaBloomFSX, /* 6*/ kPassIndexAlphaBloomFSX);
	renderPasses.addRenderPass(kRegularPass, 0x7000, 0, /* 6*/ kRenderFunctionPredator,      /* 7*/ kPassIndexPredator);
	renderPasses.addRenderPass(kRegularPass, 0x6000, 1, /* 8*/ kRenderFunction8,             /* 8*/ kPassIndex8);
	renderPasses.addRenderPass(kShadowPass,  0x8000, 1, /* 2*/ kRenderFunctionShadow,        /* 9*/ kPassIndexShadow);
	renderPasses.addRenderPass(kRegularPass, 0x1850, 2, /* 0*/ kRenderFunctionDefault,       /*10*/ kPassIndexDepthDependent);
	renderPasses.addRenderPass(kRegularPass, 0x1800, 1, /*10*/ kRenderFunctionNormal,        /*12*/ kPassIndexNormal);
	renderPasses.addRenderPass(kRegularPass, 0x1900, 2, /* 5*/ kRenderFunctionTranslucent,   /*13*/ kPassIndexDeferredShading);
	renderPasses.addRenderPass(kRegularPass, 0x1400, 1, /* 1*/ kRenderFunctionDepth,         /*14*/ kPassIndexNonNormalDepth);
	setupPassCallbacks();
	registerComparatorsAndDrawersModel();
	registerComparatorsAndDrawersTerrain();
	registerComparatorsAndDrawersNGAPrim();
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
	std::vector<VertexAttributeA> attrs;
	// hash, offset, format, bufferIndex
	attrs.push_back({VertexAttributeA::kPosition,  0xffff, 2, 0});
	attrs.push_back({VertexAttributeA::kColor1,    0xffff, 4, 0});
	attrs.push_back({VertexAttributeA::kTexcoord1, 0xffff, 1, 0});
	attrs.push_back({VertexAttributeA::kTexcoord2, 0xffff, 1, 0});
	drawVertexDecls[0] = VertexDecl::Create(attrs.data(), attrs.size(), /*stride=*/ 0);

	attrs.clear();
	attrs.push_back({VertexAttributeA::kPosition,  0xffff, 2, 0});
	attrs.push_back({VertexAttributeA::kColor1,    0xffff, 4, 0});
	attrs.push_back({VertexAttributeA::kTexcoord0, 0xffff, 1, 0}); // HACK
	drawVertexDecls[5] = VertexDecl::Create(attrs.data(), attrs.size(), /*stride=*/ 0);

	attrs.clear();
	attrs.push_back({VertexAttributeA::kPosition,  0xffff, 2, 0});
	drawVertexDecls[6] = VertexDecl::Create(attrs.data(), attrs.size(), /*stride=*/ 0);

	auto *vs = static_cast<PCDX11VertexShaderTable*>(shlib_21->table)->vertexShaders[0];
	vertex2DStreamDecl = streamDeclCache.buildStreamDecl(drawVertexDecls[5], &vs->m_sub);
}

void PCDX11RenderDevice::setupPassCallbacks() {
	setPassCallback(/* 0*/ kPassIndexDepth,           &depthPassCallbacks);
	setPassCallback(/*14*/ kPassIndexNonNormalDepth,  &depthPassCallbacks);
	setPassCallback(/*10*/ kPassIndexDepthDependent,  &depthDependentPassCallbacks);
	setPassCallback(/* 1*/ kPassIndexComposite,       &compositePassCallbacks);
	setPassCallback(/* 2*/ kPassIndexOpaque,          &opaquePassCallbacks);
	setPassCallback(/* 3*/ kPassIndexTranslucent,     &translucentPassCallbacks);
	setPassCallback(/* 6*/ kPassIndexAlphaBloomFSX,   &alphaBloomFSXPassCallbacks);
	setPassCallback(/* 7*/ kPassIndexPredator,        &predatorPassCallbacks);
	setPassCallback(/* 4*/ kPassIndexFullScreenFX,    &fullScreenFXPassCallbacks);
	setPassCallback(/* 5*/ kPassIndexPostFSX,         &postFSXPassCallbacks);
	setPassCallback(/*12*/ kPassIndexNormal,          &normalPassCallbacks);
	setPassCallback(/*13*/ kPassIndexDeferredShading, &deferredShadingPassCallbacks);
}

void PCDX11RenderDevice::registerComparatorsAndDrawersModel() {
	allocFuncIndex("RenderModelDrawable");
	uint8_t funcIndex = 1;
	registerComparator(funcIndex, /*1*/ kRenderFunctionDepth,         PCDX11ModelDrawable::compare128);
	registerComparator(funcIndex, /*2*/ kRenderFunctionShadow,        PCDX11ModelDrawable::compare128);
	registerComparator(funcIndex, /*8*/ kRenderFunction8,             PCDX11ModelDrawable::compare128);
	registerComparator(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, PCDX11ModelDrawable::compare7);
	registerComparator(funcIndex, /*4*/ kRenderFunctionComposite,     PCDX11ModelDrawable::compare46);
	registerComparator(funcIndex, /*6*/ kRenderFunctionPredator,      PCDX11ModelDrawable::compare46);

	registerDrawer(funcIndex, /*1*/ kRenderFunctionDepth,         PCDX11ModelDrawable::drawDepth);
	registerDrawer(funcIndex, /*2*/ kRenderFunctionShadow,        PCDX11ModelDrawable::drawShadow);
	registerDrawer(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, PCDX11ModelDrawable::drawAlphaBloom);
	registerDrawer(funcIndex, /*4*/ kRenderFunctionComposite,     PCDX11ModelDrawable::drawComposite);
	registerDrawer(funcIndex, /*5*/ kRenderFunctionTranslucent,   PCDX11ModelDrawable::drawTranslucent);
	registerDrawer(funcIndex, /*6*/ kRenderFunctionPredator,      PCDX11ModelDrawable::drawTranslucent);

	registerComparator(funcIndex, /*10*/ kRenderFunctionNormal, PCDX11ModelDrawable::compareNormal);
	registerDrawer(funcIndex, /*10*/ kRenderFunctionNormal, PCDX11ModelDrawable::drawNormal);
}

void PCDX11RenderDevice::registerComparatorsAndDrawersTerrain() {
	uint8_t funcIndex = allocFuncIndex("RenderTerrainDrawable");
	registerComparator(funcIndex, /*1*/ kRenderFunctionDepth, PCDX11TerrainDrawable::compare12);
	registerComparator(funcIndex, /*2*/ kRenderFunctionShadow, PCDX11TerrainDrawable::compare12);
	registerComparator(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, PCDX11TerrainDrawable::compare467);
	registerComparator(funcIndex, /*4*/ kRenderFunctionComposite, PCDX11TerrainDrawable::compare467);
	registerComparator(funcIndex, /*6*/ kRenderFunctionPredator, PCDX11TerrainDrawable::compare467);

	registerDrawer(funcIndex, /*1*/ kRenderFunctionDepth, PCDX11TerrainDrawable::draw1);
	registerDrawer(funcIndex, /*2*/ kRenderFunctionShadow, PCDX11TerrainDrawable::draw2);
	registerDrawer(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, PCDX11TerrainDrawable::draw7);
	registerDrawer(funcIndex, /*4*/ kRenderFunctionComposite, PCDX11TerrainDrawable::draw4);
	registerDrawer(funcIndex, /*5*/ kRenderFunctionTranslucent, PCDX11TerrainDrawable::draw56);
	registerDrawer(funcIndex, /*6*/ kRenderFunctionPredator, PCDX11TerrainDrawable::draw56);

	registerComparator(funcIndex, /*10*/ kRenderFunctionNormal, PCDX11TerrainDrawable::compareA);
	registerDrawer(funcIndex, /*10*/ kRenderFunctionNormal, PCDX11TerrainDrawable::drawA);
}

void PCDX11RenderDevice::registerComparatorsAndDrawersNGAPrim() {
	DrawableCompareFn todoCmp = nullptr;

	DrawableRenderFn todoDraw1 = nullptr;
	DrawableRenderFn todoDraw2 = nullptr;
	DrawableRenderFn todoDraw7 = nullptr;
	DrawableRenderFn todoDraw4 = nullptr;
	DrawableRenderFn todoDraw56 = nullptr;
	DrawableRenderFn todoDrawA = nullptr;

	uint8_t funcIndex = allocFuncIndex("RenderTerrainDrawable");
	registerComparator(funcIndex, /*1*/ kRenderFunctionDepth, todoCmp);
	registerComparator(funcIndex, /*2*/ kRenderFunctionShadow, todoCmp);
	registerComparator(funcIndex, /*8*/ kRenderFunction8, todoCmp);
	registerComparator(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, todoCmp);
	registerComparator(funcIndex, /*4*/ kRenderFunctionComposite, todoCmp);
	registerComparator(funcIndex, /*6*/ kRenderFunctionPredator, todoCmp);

	registerDrawer(funcIndex, /*1*/ kRenderFunctionDepth, todoDraw1);
	registerDrawer(funcIndex, /*2*/ kRenderFunctionShadow, todoDraw2);
	registerDrawer(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, todoDraw7);
	registerDrawer(funcIndex, /*4*/ kRenderFunctionComposite, todoDraw4);
	registerDrawer(funcIndex, /*5*/ kRenderFunctionTranslucent, todoDraw56);
	registerDrawer(funcIndex, /*6*/ kRenderFunctionPredator, todoDraw56);

	registerComparator(funcIndex, /*10*/ kRenderFunctionNormal, todoCmp);
	registerDrawer(funcIndex, /*10*/ kRenderFunctionNormal, todoDrawA);
}

void PCDX11RenderDevice::setupShadowBuffer() {
	// TODO
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

bool PCDX11RenderDevice::beginRenderList(float *scale) {

	uint32_t dimensions[4];
	uint32_t *dimensionsPtr = nullptr;

	if (scale) {
		auto *renderContext = getRenderContextAny();
		uint32_t width = renderContext->width; // via method
		uint32_t height = renderContext->height; // via method
		float fwidth = (float)width;
		float fheight = (float)height;
		int64_t scaledWidth = scale[0] * fwidth;
		int64_t scaledHeight = scale[0] * fheight;
		double widthLimit = fwidth * scale[2] + 0.5;
		double heightLimit = fheight * scale[3] + 0.5;

		dimensions[0] = scaledWidth;
		dimensions[1] = scaledHeight;
		dimensions[2] = width - scaledWidth;
		dimensions[3] = height - scaledHeight;

		if (dimensions[2] >= widthLimit)
			dimensions[2] = widthLimit;

		if (dimensions[3] >= heightLimit)
			dimensions[3] = heightLimit;

		dimensionsPtr = dimensions;
	}

	renderList_current = new (linear30, 0, true) RenderList(this, dimensionsPtr);
	auto *lightManager = static_cast<PCDX11LightManager*>(this->lightManager);
	renderList_current->lightManagerSubB = lightManager->allocateSubB();
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

uint32_t PCDX11RenderDevice::getSubFrameWidth() {
	// called from PCDX11RenderTexture::UpdateAbsoluteSize
	// TODO
	return 0;
}

uint32_t PCDX11RenderDevice::getSubFrameHeight() {
	// called from PCDX11RenderTexture::UpdateAbsoluteSize
	// TODO
	return 0;
}

void PCDX11RenderDevice::method_30() {
	// TODO
}

CommonScene *PCDX11RenderDevice::createSubScene(
	RenderViewport *viewport,
	CommonRenderTarget *renderTarget,
	CommonDepthBuffer *depthBuffer
) {
	// TODO
	auto scene = new (linear30, 1, true) PCDX11Scene(
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
	if (passMask == 0)
		return;

	uint32_t clearColorI =
		((int)(clearColor[3] * 255.0) << 24) |
		((int)(clearColor[0] * 255.0) << 16) |
		((int)(clearColor[1] * 255.0) <<  8) |
		((int)(clearColor[2] * 255.0) <<  0);
	auto clearDrawable = new (linear30, 1, true) PCDX11ClearDrawable(
		sortOrder,
		this,
		(flags & 2 | (flags >> 1) & 0xC) >> 1,
		clearColorI,
		clearDepth,
		clearStencil);
	recordDrawable(clearDrawable, passMask, 0);
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

IMaterial *PCDX11RenderDevice::createMaterial() {
	return new PCDX11Material(this);
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

RenderMesh *PCDX11RenderDevice::createRenderModel(uint32_t size) {
	return new PCDX11RenderModel(this, size);
}

RenderModelInstance *PCDX11RenderDevice::createRenderModelInstance(RenderMesh *renderMesh) {
	return new PCDX11RenderModelInstance(static_cast<PCDX11RenderModel*>(renderMesh), this);
}

IRenderTerrain *PCDX11RenderDevice::createRenderTerrain(uint32_t size) {
	return new PCDX11RenderTerrain(this, size);
}

IRenderTerrainInstance *PCDX11RenderDevice::createRenderTerrainInstance(IRenderTerrain *renderTerrain) {
	return new CommonRenderTerrainInstance(static_cast<PCDX11RenderTerrain*>(renderTerrain));
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

IMatrixState *PCDX11RenderDevice::createMatrixState() {
	return new PCDX11MatrixState(this);
}

void PCDX11RenderDevice::createVertexBuffer() {
	// TODO
}

void PCDX11RenderDevice::createIndexBuffer() {
	// TODO
}

IRenderTarget *PCDX11RenderDevice::createRenderTarget(
	uint32_t width,
	uint32_t height,
	uint32_t flags,
	uint32_t cdcFormat,
	uint32_t ignored6,
	uint32_t arg7
) {
	uint32_t dxgiFormat = decodeFormat(cdcFormat);
	if (dxgiFormat == 0)
		dxgiFormat = 0x1C; // DXGI_FORMAT_R8G8B8A8_UNORM
	return dx11_createRenderTarget(width, height, dxgiFormat, flags, arg7);
}

IDepthBuffer *PCDX11RenderDevice::createDepthBuffer() {
	// TODO
	return nullptr;
}

void PCDX11RenderDevice::method_174() {
	// TODO
}

void PCDX11RenderDevice::getWindowSize(uint32_t *width, uint32_t *height) {
	// TODO
}

void PCDX11RenderDevice::method_17C() {
	// TODO
}

void PCDX11RenderDevice::method_180() {
	// TODO
}

struct Capture {
	LinearAllocator *linear;
	PCDX11RenderDevice::RenderList *list;
	~Capture() { delete linear; }
};

void *PCDX11RenderDevice::captureRenderLists() {
	auto *capture = new Capture;
	capture->linear = linear30; // steal the allocator
	linear30 = new LinearAllocator(0x300000, false, "RenderDevice"); // replace with a fresh one
	capture->list = renderList_first;
	renderList_override = renderList_first;
	renderList_first = nullptr;
	return (void*)capture;
}

void PCDX11RenderDevice::revisitRenderLists(void *capture) {
	if (capture)
		renderList_override = reinterpret_cast<Capture*>(capture)->list;
	else
		renderList_override = nullptr;
}

void PCDX11RenderDevice::freeRenderLists(void *capture) {
	delete reinterpret_cast<Capture*>(capture);
}

bool PCDX11RenderDevice::internalCreate() {
	deviceContext = deviceManager->getD3DDeviceContext();
	// TODO
	// fullScreenQuadVB = new PCDX11SimpleStaticVertexBuffer(/*stride=*/ 12, /*count=*/ 4, data);
	quadVB = new PCDX11SimpleDynamicVertexBuffer(/*stride=*/ 32, /*count=*/ 4);
	setupShadowBuffer();
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

PCDX11RenderContext *PCDX11RenderDevice::getRenderContextAny() {
	if (renderContext_10CE8)
		return renderContext_10CE8;
	else
		return renderContext_10CEC;
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

PCDX11RenderTarget *PCDX11RenderDevice::dx11_createRenderTarget(
	uint32_t width,
	uint32_t height,
	uint32_t dxgiFormat,
	uint32_t flags,
	uint32_t textureClass)
{
	if (flags & 0x10) {
		auto *linear = useAlternateLinearAlloc() ? linear34 : linear30;
		return new (linear, 12, true)
			PCDX11DefaultRenderTarget(width, height, flags & ~4, dxgiFormat, this, 0, textureClass);

	} else {
		// return new PCDX11HeapRenderTarget(width, height, flags, dxgiFormat, this, textureClass);
		return nullptr;
	}
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

void PCDX11RenderDevice::handleResize(int32_t width, int32_t height) {
	deviceManager->method_1C();

	if (width < 1) width = 1;
	if (height < 1) height = 1;

	if (!deviceManager->getDisplayConfig()->lockWindowResolution && renderContext_10CEC) {
		if (d3dDeviceContext111580)
			d3dDeviceContext111580->ClearState();

		renderContext_10CEC->handleResize(width, height);
	}
}

void PCDX11RenderDevice::dx11_method_48() {
	// TODO
}

PCDX11LightManager *PCDX11RenderDevice::getLightManager() {
	return static_cast<PCDX11LightManager*>(lightManager);
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
		ID3D11DepthStencilView *view = depthBuffer->getRWDepthStencilView();
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
	if (arg) {
		// TODO
		auto stateManager = deviceManager->getStateManager();
		stateManager->reset();
		stateManager->setCommonConstantBuffers();

		static_cast<PCDX11LightManager*>(lightManager)->setAttenuationSampler();
		while (renderList_processing) {
			// TODO
			getLightManager()->renderLights(renderList_processing->lightManagerSubB);
			renderList_processing->drawableList.draw(renderPasses.drawers, /*funcSetIndex=*/0);
			renderList_processing = renderList_processing->next;
		}
	}

	freeTemporarySurfaces();
}

void PCDX11RenderDevice::drawQuadInternal(
	float x0, float y0, float x1, float y1,
	float u0, float v0, float u1, float v1,
	uint32_t color)
{
	struct Vertex { float x, y, unk; uint32_t color; float u0, v0, u1, v1; };
	static_assert(sizeof(Vertex) == 32, "sizeof(Vertex) == 32");
	float unk = 0.0001;
	Vertex v[4] = {
		{x1, y0, unk, color, u1, v1, u1, v1},
		{x0, y0, unk, color, u0, v1, u0, v1},
		{x1, y1, unk, color, u1, v0, u1, v0},
		{x0, y1, unk, color, u0, v0, u0, v0}
	};

	memcpy(quadVB->map(), v, sizeof(Vertex) * 4);
	quadVB->unmap();

	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	stateManager->setVertexBuffer(quadVB);
	deviceContext->Draw(4, 0);
}

void PCDX11RenderDevice::drawQuad(
	float x0, float y0, float x1, float y1,
	float u0, float v0, float u1, float v1,
	uint32_t color, uint32_t flags, uint32_t blendMode, bool writeDepth)
{
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	stateManager->setDepthState(D3D11_COMPARISON_ALWAYS, writeDepth);
	stateManager->setRenderTargetWriteMask(15);
	stateManager->setBlendStateAndBlendFactors(blendMode, 1, 0);
	stateManager->setDepthBias(0);
	stateManager->setSlopeScaledDepthBias(0.0f);
	stateManager->setStreamDecl(vertex2DStreamDecl);
	stateManager->setPrimitiveTopology(5);
	stateManager->setCullMode(CullMode::none, false);
	stateManager->updateViewport();
	stateManager->updateRenderState();
	drawQuadInternal(x0, y0, x1, y1, u0, v0, u1, v1, color);
}

void PCDX11RenderDevice::doFXAA(
	uint32_t quality,
	PCDX11Texture *texture,
	PCDX11RenderTarget *renderTarget)
{
	if (!scene78) {
		printf("scene78 = nullptr\n");
		return;
	}
	if (!scene78->globalState.tex14[5+6]) {
		printf("scene78->globalState.tex14[5+6] = nullptr\n");
		return;
	}

	// HACK
	texture = static_cast<PCDX11Texture*>(scene78->globalState.tex14[5+6]);
	renderTarget = static_cast<PCDX11RenderTarget*>(scene78->renderTarget);
	// END HACK

	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	stateManager->pushRenderTargets(renderTarget, nullptr);
	if (quality > 2) quality = 2; // allow 0, 1 and 2

	auto *vs = static_cast<PCDX11VertexShaderTable*>(shlib_19->table)->vertexShaders[0];
	stateManager->setVertexShader(vs);
	auto *ps = static_cast<PCDX11PixelShaderTable*>(shlib_12->table)->pixelShaders[quality];
	stateManager->setPixelShader(ps);
	stateManager->setTextureAndSampler(0, texture, 1, 0.0f);
	stateManager->updateViewport();
	stateManager->updateRenderState();
	drawQuad(
		-1.0f, -1.0f, 1.0f, 1.0f,
		 0.0f,  0.0f, 1.0f, 1.0f,
		/*color=*/      0xffffffff,
		/*flags=*/      0,
		/*blendMode=*/  0x7010010,
		/*writeDepth=*/ false
	);
	stateManager->popRenderTargets();
}


CommonRenderDevice *createPCDX11RenderDevice(HWND hwnd, uint32_t width, uint32_t height, bool unknown) {
	// createPCDX11DeviceManager(); // already done, else wouldn't have an hwnd
	g_renderDevice = new PCDX11RenderDevice(hwnd, width, height/*, unknown*/);
	return g_renderDevice;
}

}
