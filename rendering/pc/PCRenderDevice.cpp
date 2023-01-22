#include "PCMaterial.h"
#include "PCRenderDevice.h"
#include "shaders/PCShaderLib.h"

namespace cdc {

PCRenderDevice::PCRenderDevice(HWND hwnd, uint32_t width, uint32_t height) {
	// TODO
	renderContext = CreateRenderContext(hwnd, width, height, /*useMultiSapmle=*/ true);
	// TODO
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
	// TODO
}

void PCRenderDevice::drawRenderLists() {
	// TODO
}

bool PCRenderDevice::beginRenderList(float*) {
	// TODO
	return true;
}

bool PCRenderDevice::endRenderList() {
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
	RenderViewport *renderViewport,
	CommonRenderTarget *renderTarget,
	CommonDepthBuffer *depthBuffer)
{
	// TODO
	return nullptr;
}

void PCRenderDevice::finishScene() {
	// TODO
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
	// TODO
	return nullptr;
}

void PCRenderDevice::createProceduralTexture() {
	// TODO
}

IShaderLib *PCRenderDevice::createShaderLib(uint32_t size) {
	return new PCShaderLib(size, this);
}

RenderMesh *PCRenderDevice::createRenderModel(uint32_t) {
	// TODO
	return nullptr;
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

CommonRenderDevice *createPCRenderDevice(HWND hwnd, uint32_t width, uint32_t height, bool unknown) {
	// HACK
	g_renderDevice = new PCRenderDevice(hwnd, width, height/*, unknown*/);
	return g_renderDevice;
}

}
