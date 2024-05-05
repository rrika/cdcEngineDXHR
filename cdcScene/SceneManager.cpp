#include "cdcScene.h"
#include "Scene.h"
#include "SceneCell.h"
#include "SceneCellContainer.h"
#include "SceneCellGroup.h"
#include "SceneDynamicPortal.h"
#include "SceneEntity.h"
#include "SceneLight.h"
#include "SceneManager.h"

namespace cdc {

SceneManager *g_sceneManager = nullptr;

SceneManager *SceneManager::create() {
	if (!g_sceneManager)
		g_sceneManager = new SceneManager();
	return g_sceneManager;
}

IScene *SceneManager::createScene(CommonRenderDevice *renderDevice /*, TODO*/) {
	return new Scene(renderDevice);
}

}
