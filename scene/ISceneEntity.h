#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class IDrawable;

class ISceneEntity {
public:
	// 25 methods
	virtual void setMatrix(Matrix&) = 0; // 4
	virtual Matrix& getMatrix() = 0; // 8
	virtual void setDrawable(IDrawable *) = 0; // C
	virtual IDrawable *getDrawable() = 0; // 10
};

}
