#include "SceneLayer.h"
#include "../scene/SceneManager.h"

namespace cdc {
	class CommonRenderDevice;
	extern CommonRenderDevice *g_renderDevice;
}

cdc::Scene *g_scene = nullptr;

void SceneLayer::init() {
	auto *sceneManager = cdc::SceneManager::create();
	sceneManager->createScene(cdc::g_renderDevice);
}
