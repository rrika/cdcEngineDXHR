#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc {

class IDrawable;
class ISceneCellGroup;
class ISceneEntity;
struct CellGroupData;
struct RenderViewport;

class IScene { // line 206
public:
	// 42 methods
	virtual void Render(/*TODO*/) = 0; // 4
	virtual void RenderWithoutCellTracing(RenderViewport& viewport /*TODO*/) = 0; // 8
	virtual ISceneCellGroup *CreateCellGroup(CellGroupData*) = 0;
	virtual ISceneCellGroup *GetCellGroup(uint32_t) = 0; // 24
	virtual ISceneEntity *CreateEntity() = 0; // 28
};

class ISceneCell { // line 498
	// 27 methods
};

class ISceneCellGroup { // line 675
public:
	// 23 methods
	virtual IScene *getScene() = 0; // 4
	virtual uint32_t getCellCount() = 0; // 8
	virtual ISceneCell *cellByIndex(uint32_t index) = 0; // 10
	virtual ISceneCell *GetCellFromPoint(Vector3 const& point, bool ignoreChildCellContainers) = 0; // 14
	virtual Vector3 *getOrigin() = 0; // 20
	virtual void setUserData(void *) = 0; // 38
	virtual void setName(const char *) = 0; // 48
	virtual const char *getName() = 0; // 4C
};


class ISceneEntity { // line 819
public:
	struct UpdateState {
		static const uint32_t kEnabled = 1;
		static const uint32_t kDrawable = 2;
		static const uint32_t kCellGroup = 4;
		static const uint32_t kMatrix = 8;
		static const uint32_t kMoveState = 16;
		static const uint32_t kUpdateVolume = 32;
		static const uint32_t kUpdateFlags = 64;
		static const uint32_t kUpdateAll = 127;

		uint32_t updateFlags;
		bool enabled;
		IDrawable *drawable;
		ISceneCellGroup *cellGroup;
		Matrix matrix;
		uint32_t moveState;
	};

	// 25 methods
	virtual void ApplyUpdateState(UpdateState*) = 0; // 0
	virtual void setMatrix(Matrix&) = 0; // 4
	virtual Matrix& getMatrix() = 0; // 8
	virtual void setDrawable(IDrawable *) = 0; // C
	virtual IDrawable *getDrawable() = 0; // 10
	virtual void setCellGroup(ISceneCellGroup *) = 0; // 38
	virtual void Release() = 0;	// 58
	virtual ~ISceneEntity() = default; // 5C
};

class ISceneLight : public ISceneEntity { // line 1036
	// 40 methods
};

class IScenePortal { // line 1262
	// 7 methods
};

}
