#pragma once
#include <vector>
#include "IScene.h"

namespace cdc {

class CommonRenderDevice;
class SceneEntity;
class SceneCell;
class SceneCellGroup;
class SceneEntity;

class Scene : public IScene {
	CommonRenderDevice *renderDevice; // 4
	SceneCell *sceneCell; // B8
	std::vector<SceneEntity*> activeEntities; // 2F0
public:
	Scene(CommonRenderDevice *renderDevice);

	// for DoRender
	void DrawEntities(/*TODO*/);

	// 43 methods
	void Draw() override;
	SceneEntity *CreateEntity() override;
};

}
