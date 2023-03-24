#pragma once
#include <cstdint>

namespace cdc {

class SceneCellGroup;
class SceneEntity;
struct RenderViewport;

class IScene {
public:
	// 42 methods
	virtual void Render(/*TODO*/) = 0; // 4
	virtual void RenderWithoutCellTracing(RenderViewport& viewport /*TODO*/) = 0; // 8
	virtual SceneCellGroup *GetCellGroup(uint32_t) = 0; // 24
	virtual SceneEntity *CreateEntity() = 0; // 28
};

}
