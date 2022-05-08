#include "BuiltinResources.h"
#include "PCDX11ClearDrawable.h"
#include "PCDX11DepthBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderTarget.h"
#include "PCDX11StateManager.h"
#include "PCDX11Texture.h"

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

void PCDX11RenderDevice::clearRenderTarget(
	uint32_t flags,
	uint32_t unknown1,
	float unknown2,
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
	(void)unknown2;
	auto clearDrawable = new PCDX11ClearDrawable(
		this,
		(flags & 2 | (flags >> 1) & 0xC) >> 1,
		clearColorI,
		clearDepth,
		clearStencil);
	recordDrawable(clearDrawable, unknown1, 0);
	delete clearDrawable; // since it's not actually getting queued
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

bool PCDX11RenderDevice::internalCreate() {
	// TODO
	return true;
}

void PCDX11RenderDevice::internalRelease() {
	// TODO
}

void PCDX11RenderDevice::recordDrawable(IRenderDrawable *drawable, uint32_t arg1, uint8_t arg2) {
	// TODO
	drawable->renderDrawable0(); // hack
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
