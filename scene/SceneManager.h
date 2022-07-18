#pragma once

namespace cdc {

class CommonRenderDevice;
class IScene;

class SceneManager {
public:
	static SceneManager *create();
	IScene *createScene(CommonRenderDevice *renderDevice /*, TODO*/);
};

extern SceneManager *g_sceneManager;

}
