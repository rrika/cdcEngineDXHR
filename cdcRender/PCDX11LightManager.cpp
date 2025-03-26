#include "PCDX11DeviceManager.h"
#include "PCDX11LightManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11StateManager.h"
#include "surfaces/PCDX11BitmapTexture.h" // for cast

namespace cdc {

void PCDX11DeferredLight::RenderShadowMap(const uint32_t*, uint32_t) {
	// TODO
}

void PCDX11DeferredLight::RenderShadowMapSpot(const uint32_t*, uint32_t) {
	// TODO
}

void PCDX11DeferredLight::RenderShadowMapDirectional(const uint32_t*, uint32_t) {
	// TODO
}

void PCDX11LightManager::ShadowMapPool::Render(PCDX11LightManager *lightManager) {
	// TODO
}

PCDX11LightManager::PCDX11LightManager(CommonRenderDevice *commonRenderDevice) :
	renderDevice(static_cast<PCDX11RenderDevice*>(commonRenderDevice))
{
	attenuationSampler430 = new PCDX11BitmapTexture(renderDevice, 0, 0, 256, kTextureClassUnknown);
}

PCDX11LightManager::~PCDX11LightManager() {
	delete attenuationSampler430;
}

PCDX11LightManagerData *PCDX11LightManager::BeginSubFrame() {
	// TODO
	return nullptr;
}

void PCDX11LightManager::ApplySinglePassLightsInternal(PCDX11LightSet *receiverData) {

	auto stateManager = deviceManager->getStateManager();
	stateManager->setPsConstantBuffer(4, &staticConstantBuffer438);
	// staticConstantBuffer438.assignRow();

	auto &lightBuffer = staticConstantBuffer438;

	uint32_t zero[] = {0, 0, 0, 0};
	lightBuffer.assignRow(62, zero, 1); // ShadowLightEnabled[0]
	lightBuffer.assignRow(63, zero, 1); // ShadowLightEnabled[1]
	lightBuffer.assignRow(64, zero, 1); // ShadowLightEnabled[2]
	lightBuffer.assignRow(65, zero, 1); // NonShadowLightEnabled[0]
	lightBuffer.assignRow(66, zero, 1); // NonShadowLightEnabled[1]
	lightBuffer.assignRow(67, zero, 1); // NonShadowLightEnabled[2]
	lightBuffer.assignRow(68, zero, 1); // SunLightEnabled

	return;

	// TODO
	renderDevice->setTexture(11, receiverData->GlobalModulationMapTextures[0], 256, 0.0f);
	renderDevice->setTexture(12, receiverData->GlobalModulationMapTextures[1], 256, 0.0f);
	renderDevice->setTexture(13, receiverData->GlobalModulationMapTextures[2], 256, 0.0f);
	// TODO
}

PCDX11LightSet *PCDX11LightManager::BuildLightState() { // new light receiver
	// TODO
	return nullptr;
}

void PCDX11LightManager::BeginFlush(PCDX11LightManagerData *subB) {
	// TODO
	deviceManager->getStateManager()->setTextureAndSampler(14, nullptr, 256, 0.0f);
	// TODO
}

void PCDX11LightManager::ApplyIrradianceStateInternal(char *src) { // assign cb5
	// TODO
}

void PCDX11LightManager::UpdateCombinedLightTexture() {
	deviceManager->getStateManager()->setTextureAndSampler(15, attenuationSampler430, 1, 0.0f);
}

bool PCDX11LightManager::internalCreate() {
	pixelShaderIndexBaseE20 = 0;
	pixelShaderIndexBaseE1C = 0;
	internalOnSettingsChanged();
	// depthBufferE18 = new PCDX11DepthBuffer(4096, 3072, /*...*/)
	return true;
}

void PCDX11LightManager::internalRelease() {
	// TODO
}

void PCDX11LightManager::internalOnSettingsChanged() {
	auto mode = deviceManager->getDisplayConfig()->shadowMode;
	if (mode == 0) {
		pixelShaderIndexBaseE20 = 0;
		pixelShaderIndexBaseE1C = 0;
	} else if (mode == 1) {
		pixelShaderIndexBaseE20 = 1;
		pixelShaderIndexBaseE1C = 1;
	} else  if (mode == 2) {
		pixelShaderIndexBaseE20 = 2;
		pixelShaderIndexBaseE1C = 1;
	}
}

}
