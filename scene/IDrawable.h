#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class IDrawable {
public:
	// 15 methods
	virtual void draw(Matrix *, float) = 0; // 1C
};

}