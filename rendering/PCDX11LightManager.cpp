#include "PCDX11DeviceManager.h"
#include "PCDX11LightManager.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11StateManager.h"
#include "surfaces/PCDX11BitmapTexture.h" // for cast

namespace cdc {

void RenderLightData::render(void*, void*) {
	// TODO
}

void RenderLightData::render1(void*, void*) {
	// TODO
}

void RenderLightData::render2(void*, void*) {
	// TODO
}

void LightManagerSubC::renderLights(PCDX11LightManager *lightManager) {
	// TODO
}

PCDX11LightManager::PCDX11LightManager(CommonRenderDevice *commonRenderDevice) :
	renderDevice(static_cast<PCDX11RenderDevice*>(commonRenderDevice))
{
	attenuationSampler430 = new PCDX11BitmapTexture(renderDevice, 0, 0, 256, 0);
}

LightManagerSubB *PCDX11LightManager::allocateSubB() {
	// TODO
	return nullptr;
}

void PCDX11LightManager::fillLightBuffer(LightReceiverData *receiverData) {
	// TODO
	renderDevice->setTexture(11, receiverData->GlobalModulationMapTextures[0], 256, 0.0f);
	renderDevice->setTexture(12, receiverData->GlobalModulationMapTextures[1], 256, 0.0f);
	renderDevice->setTexture(13, receiverData->GlobalModulationMapTextures[2], 256, 0.0f);
	// TODO
}

LightReceiverData *PCDX11LightManager::makeReceiver() {
	// TODO
	return nullptr;
}

void PCDX11LightManager::renderLights(LightManagerSubB *subB) {
	// TODO
	deviceManager->getStateManager()->setTextureAndSampler(14, nullptr, 256, 0.0f);
	// TODO
}

void PCDX11LightManager::assignCommonCB5(char *src) {
	// TODO
}

void PCDX11LightManager::setAttenuationSampler() {
	deviceManager->getStateManager()->setTextureAndSampler(15, attenuationSampler430, 1, 0.0f);
}

}
