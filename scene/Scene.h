#pragma once
#include "IScene.h"

namespace cdc {

class CommonRenderDevice;
class SceneCell;

class Scene : public IScene {
	CommonRenderDevice *renderDevice; // 4
	SceneCell *sceneCell; // B8
public:
	// 43 methods
};

}
