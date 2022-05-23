#pragma once
#include "ISceneEntity.h"

namespace cdc {

class SceneCellGroup;

class SceneEntity : public ISceneEntity {
	// IDrawable *drawable; // 6C
	SceneCellGroup *sceneCellGroup; // 70
public:
	// 25 methods
};

}
