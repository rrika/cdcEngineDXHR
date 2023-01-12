#pragma once
#include <cstdint>

namespace cdc {

class SceneCellGroup;
class SceneEntity;

class IScene {
public:
	// 42 methods
	virtual void Draw(/*TODO*/) = 0; // 4
	virtual SceneCellGroup *GetCellGroup(uint32_t) = 0; // 24
	virtual SceneEntity *CreateEntity() = 0; // 28
};

}
