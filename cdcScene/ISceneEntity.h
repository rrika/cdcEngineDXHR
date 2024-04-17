#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc {

class IDrawable;
class ISceneCellGroup;

class ISceneEntity {
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
};

}
