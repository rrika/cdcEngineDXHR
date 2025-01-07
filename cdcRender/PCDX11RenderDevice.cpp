#include "BuiltinResources.h"
#include "drawables/PCDX11ClearDrawable.h"
#include "drawables/PCDX11FastBlurDrawable.h"
#include "LinearAllocator.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11ImmediateDraw.h"
#include "PCDX11LightManager.h"
#include "PCDX11Material.h"
#include "PCDX11MatrixState.h"
#include "PCDX11ModelDrawable.h"
#include "PCDX11Pool.h"
#include "PCDX11RenderContext.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderModel.h"
#include "PCDX11RenderModelInstance.h"
#include "PCDX11RenderPassCallbacks.h"
#include "PCDX11RenderTerrain.h"
#include "CommonRenderTerrainInstance.h"
#include "PCDX11Scene.h"
#include "PCDX11StateManager.h"
#include "PCDX11StaticPool.h"
#include "PCDX11TerrainDrawable.h"
#include "RenderPasses.h"
#include "Types.h"
#include "buffers/PCDX11DynamicIndexBuffer.h"
#include "buffers/PCDX11DynamicVertexBuffer.h"
#include "buffers/PCDX11SimpleDynamicVertexBuffer.h"
#include "buffers/PCDX11SimpleStaticVertexBuffer.h"
#include "shaders/PCDX11ShaderLib.h"
#include "surfaces/PCDX11DefaultRenderTarget.h"
#include "surfaces/PCDX11DepthBuffer.h"
#include "surfaces/PCDX11HeapRenderTarget.h"
#include "surfaces/PCDX11RenderTarget.h"
#include "surfaces/PCDX11RenderTexture.h"
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

		auto *renderContext = renderDevice->getRenderContextAny();
		auto *currentRenderTarget = renderContext->getRenderTarget();
		renderTarget = new PCDX11SubFrameRenderTarget(
			dimensions[0],
			dimensions[1],
			dimensions[2],
			dimensions[3],
			renderDevice,
			renderTarget);
		depthBuffer = renderContext->depthBuffer; // not cropped apparently

	} else {
		auto *renderContext = renderDevice->getRenderContextAny();
		renderTarget = renderContext->getRenderTarget();
		depthBuffer = renderContext->getDepthBuffer();
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

	m_pDynamicVertexPool = new PCDX11Pool(0, D3D11_BIND_VERTEX_BUFFER, 0x100000, nullptr /*TODO*/);
	m_pDynamicIndexPool = new PCDX11Pool(0, D3D11_BIND_INDEX_BUFFER, 0x80000, nullptr /*TODO*/);
	m_pStaticVertexPool = new PCDX11StaticPool(this, D3D11_BIND_VERTEX_BUFFER, 0xA00000, nullptr /*TODO*/);
	m_pStaticIndexPool = new PCDX11StaticPool(this, D3D11_BIND_INDEX_BUFFER, 0xA00000, nullptr /*TODO*/);

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
	renderPasses.setRenderPassDebugName(kPassIndexDepth,           "Depth");
	renderPasses.setRenderPassDebugName(kPassIndexComposite,       "Composite");
	renderPasses.setRenderPassDebugName(kPassIndexOpaque,          "Opaque"); // guessed
	renderPasses.setRenderPassDebugName(kPassIndexTranslucent,     "Translucent");
	renderPasses.setRenderPassDebugName(kPassIndexFullScreenFX,    "FullScreenFX"); // guessed
	renderPasses.setRenderPassDebugName(kPassIndexPostFSX,         "PostFSX"); // guessed
	renderPasses.setRenderPassDebugName(kPassIndexAlphaBloomFSX,   "AlphaBloom");
	renderPasses.setRenderPassDebugName(kPassIndexPredator,        "Predator");
	renderPasses.setRenderPassDebugName(kPassIndex8,               "8"); // guessed
	renderPasses.setRenderPassDebugName(kPassIndexShadow,          "Shadow");
	renderPasses.setRenderPassDebugName(kPassIndexDepthDependent,  "DepthDependent"); // guessed
	renderPasses.setRenderPassDebugName(kPassIndexNormal,          "Normal");
	renderPasses.setRenderPassDebugName(kPassIndexDeferredShading, "Def. Shad");
	renderPasses.setRenderPassDebugName(kPassIndexNonNormalDepth,  "NonNormalDepth"); // guessed
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
	std::vector<VertexElem> attrs;
	// hash, offset, format, bufferIndex
	attrs.push_back({VertexElem::kPosition,  0xffff, 2, 0});
	attrs.push_back({VertexElem::kColor1,    0xffff, 4, 0});
	attrs.push_back({VertexElem::kTexcoord1, 0xffff, 1, 0});
	attrs.push_back({VertexElem::kTexcoord2, 0xffff, 1, 0});
	drawVertexDecls[0] = VertexDecl::Create(attrs.data(), attrs.size(), /*stride=*/ 0);

	attrs.clear();
	attrs.push_back({VertexElem::kPosition,  0xffff, 2, 0});
	attrs.push_back({VertexElem::kColor1,    0xffff, 4, 0});
	attrs.push_back({VertexElem::kTexcoord0, 0xffff, 1, 0}); // HACK
	drawVertexDecls[5] = VertexDecl::Create(attrs.data(), attrs.size(), /*stride=*/ 0);

	attrs.clear();
	attrs.push_back({VertexElem::kPosition,  0xffff, 2, 0});
	drawVertexDecls[6] = VertexDecl::Create(attrs.data(), attrs.size(), /*stride=*/ 0);

	PCDX11VertexShader *vs;

	vs = static_cast<PCDX11VertexShaderTable*>(shlib_21->table)->vertexShaders[0];
	vertex2DStreamDecl = streamDeclCache.buildStreamDecl(drawVertexDecls[5], &vs->m_sub);

	vs = static_cast<PCDX11VertexShaderTable*>(shlib_18->table)->vertexShaders[0];
	position3DStreamDecl = streamDeclCache.buildStreamDecl(drawVertexDecls[6], &vs->m_sub);
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

	renderPasses.bufferNames[10] = "depth buffer";
	renderPasses.bufferNames[11] = "normal buffer";
	renderPasses.bufferNames[12] = "light buffer";
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
	uint8_t funcIndex = allocFuncIndex("RenderTerrainDrawable"); // probably a copy-paste error
	registerComparator(funcIndex, /*1*/ kRenderFunctionDepth, PCDX11NGAPrimitive::compare);
	registerComparator(funcIndex, /*2*/ kRenderFunctionShadow, PCDX11NGAPrimitive::compare);
	registerComparator(funcIndex, /*8*/ kRenderFunction8, PCDX11NGAPrimitive::compare);
	registerComparator(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, PCDX11NGAPrimitive::compare);
	registerComparator(funcIndex, /*4*/ kRenderFunctionComposite, PCDX11NGAPrimitive::compare);
	registerComparator(funcIndex, /*6*/ kRenderFunctionPredator, PCDX11NGAPrimitive::compare);

	registerDrawer(funcIndex, /*1*/ kRenderFunctionDepth, PCDX11NGAPrimitive::drawDepth);
	registerDrawer(funcIndex, /*2*/ kRenderFunctionShadow, PCDX11NGAPrimitive::drawShadow);
	registerDrawer(funcIndex, /*7*/ kRenderFunctionAlphaBloomFSX, PCDX11NGAPrimitive::drawAlphaBloom);
	registerDrawer(funcIndex, /*4*/ kRenderFunctionComposite, PCDX11NGAPrimitive::drawComposite);
	registerDrawer(funcIndex, /*5*/ kRenderFunctionTranslucent, PCDX11NGAPrimitive::drawTranslucent);
	registerDrawer(funcIndex, /*6*/ kRenderFunctionPredator, PCDX11NGAPrimitive::drawTranslucent);

	registerComparator(funcIndex, /*10*/ kRenderFunctionNormal, PCDX11NGAPrimitive::compare);
	registerDrawer(funcIndex, /*10*/ kRenderFunctionNormal, PCDX11NGAPrimitive::drawNormal);
}

PCDX11RenderDevice::~PCDX11RenderDevice() {
	// HACK
	delete shlib_22;
	delete shlib_21;
	delete shlib_20;
	delete shlib_19;
	delete shlib_18;
	delete shlib_17;
	delete shlib_16;
	delete shlib_15;
	delete shlib_14;
	delete shlib_13;
	delete shlib_12;
	delete shlib_11;
	delete shlib_10;
	delete shlib_9;
	delete shlib_8;
	delete shlib_7;
	delete shlib_6;
	delete shlib_5;
	delete shlib_4;
	delete shlib_3;
	delete shlib_2;
	delete shlib_1;
	delete shlib_0;

	for (uint32_t i=0; i<std::size(drawVertexDecls); i++)
		VertexDecl::Destroy(drawVertexDecls[i]);

	delete renderContext_10CEC;
	delete static_cast<PCDX11LightManager*>(lightManager);
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

void PCDX11RenderDevice::resetRenderLists(float timeDelta) {
	// TODO
	m_frameIndex++;
	renderList_current = nullptr;
	renderList_last = nullptr;
	renderList_first = nullptr;
	// TODO
	m_frameTimeDelta = timeDelta;
	m_currentTime += 1000 * timeDelta;
	// these pools are used for data passed to DrawIndexedPrimitive
	m_pDynamicVertexPool->BeginScene();
	m_pDynamicIndexPool->BeginScene();
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

CommonScene *PCDX11RenderDevice::createSubScene( // 48
	RenderViewport *viewport,
	CommonRenderTarget *renderTarget,
	CommonDepthBuffer *depthBuffer,
	CommonRenderTarget *sourceColor,
	CommonDepthBuffer *sourceDepth
) {
	m_sceneCreationCount++;
	// TODO
	auto scene = new (linear30, 1, true) PCDX11Scene(
		this,
		scene7C, // parent scene perhaps?
		viewport,
		renderTarget,
		depthBuffer,
		sourceColor,
		sourceDepth,
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
	static_cast<PCDX11Scene*>(scene7C)->Finalize(&renderList_current->drawableList);
	scene7C = scene7C->parentScene;
}

PCDX11RenderTarget *PCDX11RenderDevice::getSceneRenderTarget() {
	if (scene7C)
		return static_cast<PCDX11RenderTarget*>(scene7C->getRenderTarget());
	return nullptr;
}

PCDX11DepthBuffer *PCDX11RenderDevice::getSceneDepthBuffer() {
	if (scene7C)
		return static_cast<PCDX11DepthBuffer*>(scene7C->getDepthBuffer());
	return nullptr;
}

uint32_t PCDX11RenderDevice::getContextWidth() {
	return getRenderContextAny()->width;
}

uint32_t PCDX11RenderDevice::getContextHeight() {
	return getRenderContextAny()->height;
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

void PCDX11RenderDevice::DrawPrimitive(
	Matrix*, TextureMap*, RenderVertex*, uint32_t, uint32_t, float)
{
	// TODO
}

void PCDX11RenderDevice::DrawIndexedPrimitive(
	PrimitiveContext *pContext,
	uint32_t startIndex,
	uint32_t numPrims,
	float sortKey)
{
	auto *state = pContext->m_pWriteState;
	uint32_t flags = state->m_flags;
	uint32_t passes = pContext->m_passes;

	if (flags & 0x1000000)
		passes = 1 << kPassIndexFullScreenFX; // 4
	else if (flags & 0x40000)
		passes = 1 << kPassIndexPostFSX; // 5

	auto *material = state->m_pMaterial;
	if (material == nullptr) {
		// TODO: obtain material from 10C38
	}

	bool fading = (flags & 0x400) || sortKey != 0.f;
	passes &= material->GetRenderPassMask(fading);

	// if (m_geometryIsVisible == false) {
	// 	passes &= ~renderPasses->dword408[kRegularPass];
	// 	passes &= ~renderPasses->dword408[kLightPass];
	// }

	if (passes) {
		flags |= 0x1000;
		if (!pContext->m_isTransient)
			pContext->NewState();

		pContext->m_dirtyBits = 0;

		PrimitiveInfo primInfo(flags, numPrims,
			/*hasSourceIndices=*/ pContext->m_pReadState->indexBuffer != nullptr);

		auto *prim = new (this, 4) PCDX11NGAPrimitive(
			pContext->m_pReadState,
			&primInfo,
			startIndex,
			sortKey,
			static_cast<PCDX11Material*>(material),
			this);
		
		// if ((passes & (1 << kPassIndexPostFSX) /*0x20*/) && ...) {
		// 	TODO
		// }
		bool drawInPartition = false; // TODO
		recordDrawable(prim, passes, drawInPartition);
		if (passes & (1 << kPassIndexShadow) /*0x200*/) {
			// TODO
			// if BlendModeIsOpaque
		}
	}
}

void PCDX11RenderDevice::DrawIndexedPrimitive(
	PrimitiveContext *pContext,
	void *verts,
	VertexDecl *vertexDecl,
	uint32_t numVertices,
	uint16_t *indices,
	uint32_t numPrims,
	float sortZ)
{
	// WIP

	auto *state = pContext->m_pWriteState;
	uint32_t flags = state->m_flags;
	uint32_t passes = pContext->m_passes;

	if (flags & 0x1000000)
		passes = 1 << kPassIndexFullScreenFX; // 4
	else if (flags & 0x40000)
		passes = 1 << kPassIndexPostFSX; // 5

	auto *material = state->m_pMaterial;
	if (material == nullptr) {
		// TODO: obtain material from 10C38
	}

	bool fading = (flags & 0x400) || sortZ != 0.f;
	passes &= material->GetRenderPassMask(fading);

	// if (m_geometryIsVisible == false) {
	// 	passes &= ~renderPasses->dword408[kRegularPass];
	// 	passes &= ~renderPasses->dword408[kLightPass];
	// }

	if (passes) {
		if ((flags & 0x1000) == 0) {
			// TODO: recalculate sortZ from matrix
		}

		if (!pContext->m_isTransient)
			pContext->NewState();

		PrimitiveInfo primInfo(flags, numPrims, /*hasSourceIndices=*/ indices != nullptr);
		if (!numVertices)
			numVertices = primInfo.m_numVertices;

		auto *vb = new (this) PCDX11DynamicVertexBuffer(m_pDynamicVertexPool, vertexDecl->vertStrideA, numVertices);
		// PrimitiveContext context2(*pContext); // probably
		PrimitiveContext& context2 = *pContext; // HACK

		context2.SetVertexDecl(vertexDecl);
		if (auto *vbmem = vb->Lock()) {
			memcpy(vbmem, verts, numVertices * vertexDecl->vertStrideA);
			context2.SetVertexBuffer(vb);
		}

		if (indices) {
			auto *ib = new (this) PCDX11DynamicIndexBuffer(m_pDynamicIndexPool, numVertices);
			auto *ibmem = ib->Lock();
			if ((flags & 0x30000) == 0x10000) {
				// TODO
			} else {
				memcpy(ibmem, indices, 2 * numVertices);
			}
			context2.SetIndexBuffer(ib);
		}

		auto *prim = new (this, 4) PCDX11NGAPrimitive(
			pContext->m_pReadState,
			&primInfo,
			/*startIndex=*/0,
			sortZ,
			static_cast<PCDX11Material*>(material),
			this);
		
		// if ((passes & (1 << kPassIndexPostFSX) /*0x20*/) && ...) {
		// 	TODO
		// }
		bool drawInPartition = false; // TODO
		recordDrawable(prim, passes, drawInPartition);
		if (passes & (1 << kPassIndexShadow) /*0x200*/) {
			// TODO
			// if BlendModeIsOpaque
		}
	}
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
	TextureClass textureClass
) {
	uint32_t dxgiFormat = decodeFormat(cdcFormat);
	if (dxgiFormat == 0)
		dxgiFormat = 0x1C; // DXGI_FORMAT_R8G8B8A8_UNORM
	return dx11_createRenderTarget(width, height, dxgiFormat, flags, textureClass);
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
	std::vector<IRenderSurface*> temporarySurfaces;
	~Capture() {
		for (auto temp : temporarySurfaces)
			temp->EvictNow();
		delete linear;
	}
};

void *PCDX11RenderDevice::captureRenderLists() {
	auto *capture = new Capture;
	capture->linear = linear30; // steal the allocator
	linear30 = new LinearAllocator(0x300000, false, "RenderDevice"); // replace with a fresh one
	capture->list = renderList_first;
	capture->temporarySurfaces = std::vector<IRenderSurface*>(
		temporarySurfaces,
		temporarySurfaces + numTemporarySurfaces);
	numTemporarySurfaces = 0;
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
	float data[] = {
		 1.0, -1.0, 0.0001,
		-1.0, -1.0, 0.0001,
		 1.0,  1.0, 0.0001,
		-1.0,  1.0, 0.0001
	};
	fullScreenQuadVB = new PCDX11SimpleStaticVertexBuffer(/*stride=*/ 12, /*count=*/ 4, data);
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
	TextureClass textureClass)
{
	if (flags & 0x10) {
		auto *linear = useAlternateLinearAlloc() ? linear34 : linear30;
		return new (linear, 12, true)
			PCDX11DefaultRenderTarget(width, height, flags & ~4, dxgiFormat, this, 0, textureClass);

	} else {
		return new PCDX11HeapRenderTarget(width, height, flags, dxgiFormat, this, textureClass);
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

PCDX11RenderTarget *PCDX11RenderDevice::GetDefaultRenderTarget(bool isFlush) { // line 2340
	RenderList *list = isFlush ? renderList_processing : renderList_current;
	return list ? list->renderTarget : nullptr;
}

PCDX11DepthBuffer *PCDX11RenderDevice::GetDefaultDepthBuffer(bool isFlush) { // line 2349
	RenderList *list = isFlush ? renderList_processing : renderList_current;
	return list ? list->depthBuffer : nullptr;
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

	m_pDynamicVertexPool->EndScene();
	m_pDynamicIndexPool->EndScene();

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
		float timeVector[4] = {
			(m_currentTime % 1000000) * 0.001f,
			m_frameTimeDelta,
			0.f,
			0.f
		};
		stateManager->accessCommonCB(1).assignRow(24, timeVector, 1); // SceneBuffer::TimeVector

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

void PCDX11RenderDevice::copySurface(
	PCDX11RenderTexture *texture, bool writeDepth, uint8_t rtMask)
{
	PCDX11StateManager *stateManager = deviceManager->getStateManager();
	stateManager->setTextureAndSampler(0, texture, 256, 0.0f);
	stateManager->setDepthState(D3D11_COMPARISON_ALWAYS, writeDepth);
	stateManager->setRenderTargetWriteMask(rtMask);
	stateManager->setBlendStateAndBlendFactors(0x7010010, 1, 0);
	stateManager->setVertexShader((*static_cast<PCDX11VertexShaderTable*>(shlib_18->table))[0]);
	stateManager->setPixelShader((*static_cast<PCDX11PixelShaderTable*>(shlib_17->table))[writeDepth]);
	// stateManager->setHullShader(nullptr);
	// stateManager->setDomainShader(nullptr);
	stateManager->setStreamDecl(position3DStreamDecl);
	stateManager->setPrimitiveTopology(5);
	stateManager->setCullMode(CullMode::none, false);
	stateManager->updateViewport();
	stateManager->updateRenderState();
	stateManager->setVertexBuffer(fullScreenQuadVB);
	deviceContext->Draw(4, 0);
}

void PCDX11RenderDevice::ApplyFXAA(
	uint32_t quality,
	PCDX11BaseTexture *texture,
	PCDX11RenderTarget *renderTarget)
{
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

void PCDX11RenderDevice::ApplyMLAA(
	PCDX11BaseTexture *tex0,
	PCDX11RenderTarget *renderTarget)
{
	PCDX11StateManager *stateManager = deviceManager->getStateManager();

	ID3D11Buffer *backupPixelCB[1];
	d3dDeviceContext111580->PSGetConstantBuffers(0, 1, backupPixelCB);

	PCDX11RenderTarget *tmp1 = dx11_createRenderTarget(100, 100, DXGI_FORMAT_R8_UINT,   /*flags=*/0x19, kTextureClass2D);
	PCDX11RenderTarget *tmp2 = dx11_createRenderTarget(100, 100, DXGI_FORMAT_R8G8_UINT, /*flags=*/0x19, kTextureClass2D);

	tmp1->getRenderTexture11()->createRenderTargetView(); // HACK
	tmp2->getRenderTexture11()->createRenderTargetView(); // HACK

	// auto *cb = new (linear34, 0) PCDX11DynamicConstantBuffer(nullptr, 1);
	auto *cb = new (linear34, 0) PCDX11UberConstantBuffer(1); // HACK
	float row[4] = {0, float(tex0->width-1), float(tex0->height-1), 0}; // game doesn't actually initialize .x and .w
	cb->assignRow(0, row, 1);
	cb->syncBuffer(d3dDeviceContext111580);

	ID3D11Buffer *buf = cb->getBuffer();
	d3dDeviceContext111580->PSSetConstantBuffers(0, 1, &buf);

	auto *vs = static_cast<PCDX11VertexShaderTable*>(shlib_19->table)->vertexShaders[0];
	stateManager->setVertexShader(vs);

	auto *ps1 = static_cast<PCDX11PixelShaderTable*>(shlib_13->table)->pixelShaders[0];
	auto *ps2 = static_cast<PCDX11PixelShaderTable*>(shlib_11->table)->pixelShaders[0];
	auto *ps3 = static_cast<PCDX11PixelShaderTable*>(shlib_10->table)->pixelShaders[0];

	// 1
	stateManager->setPrimitiveTopology(5);
	stateManager->setPixelShader(ps1);
	stateManager->setTextureAndSampler(0, tex0, 0, 0.0f);
	stateManager->pushRenderTargets(tmp1, nullptr);
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

	// 2
	stateManager->setPixelShader(ps2);
	stateManager->setTextureAndSampler(1, tmp1->getRenderTexture(), 0, 0.0f);
	stateManager->updateRenderTargets(tmp2, nullptr);
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

	// 3
	stateManager->setTextureAndSampler(0, tex0, 0, 0.0f);
	stateManager->setTextureAndSampler(1, nullptr, 0, 0.0f);
	stateManager->setTextureAndSampler(2, tmp2->getRenderTexture(), 0, 0.0f);
	stateManager->setPixelShader(ps3);
	stateManager->updateRenderTargets(renderTarget, nullptr);
	StencilParams stencilParams {
		0xFF000105,
		0xFF00000E,
		0x0000FFFF,
		0x00000000
	};
	stateManager->setStencil(&stencilParams);
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

	d3dDeviceContext111580->PSSetConstantBuffers(0, 1, backupPixelCB);

	stateManager->popRenderTargets();
}

void PCDX11RenderDevice::fastBlur(
	PCDX11BaseTexture *texture, PCDX11RenderTarget *renderTarget,
	uint32_t passMask, bool isHorizontal, /*TODO*/ bool weighted)
{
	if (passMask) {
		auto *drawable = new (linear30) PCDX11FastBlurDrawable(
			this, texture, renderTarget, isHorizontal, weighted);
		recordDrawable(drawable, passMask, 0);
	}
}


PCDX11ComputeShader *PCDX11RenderDevice::getBlurShader(bool horizontal, uint32_t kind) {
	PCDX11ShaderLib *sl;

	if (kind == 0 || kind >= 3)
		sl = shlib_4; // fastblur 0
	else if (kind == 1)
		sl = shlib_3; // fastblur 1
	else if (kind == 2)
		sl = shlib_2; // bilateral blur

	auto *cst = static_cast<PCDX11ComputeShaderTable*>(sl->table);
	return cst->computeShaders[horizontal ? 1 : 0];
}

CommonRenderDevice *createPCDX11RenderDevice(HWND hwnd, uint32_t width, uint32_t height, bool unknown) {
	// createPCDX11DeviceManager(); // already done, else wouldn't have an hwnd
	g_renderDevice = new PCDX11RenderDevice(hwnd, width, height/*, unknown*/);
	return g_renderDevice;
}

}
