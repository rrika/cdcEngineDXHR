#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11Texture.h"

namespace cdc {

PCDX11RenderDevice::PCDX11RenderDevice() {
	
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

void PCDX11RenderDevice::method_48() {
	// TODO
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

void PCDX11RenderDevice::clearRenderTarget() {
	// TODO
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

void PCDX11RenderDevice::createShader() {
	// TODO
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

void PCDX11RenderDevice::internalResource04() {
	// TODO
}

void PCDX11RenderDevice::internalResource08() {
	// TODO
}

PCDX11RenderDevice *createPCDX11RenderDevice(HWND hwnd, uint width, uint height, bool unknown) {
	// createPCDX11DeviceManager(); // already done, else wouldn't have an hwnd
	return new PCDX11RenderDevice();
}

}
