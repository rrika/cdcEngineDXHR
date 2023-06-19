#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc {

class IDrawable;
class ISceneCellGroup;

class ISceneEntity {
public:
	struct UpdateState {
		uint32_t updateFlags;
		bool enabled;
		IDrawable *drawable;
		SceneCellGroup *cellGroup;
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
};

}
