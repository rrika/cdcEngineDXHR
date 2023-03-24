#pragma once
#include "cdcMath/Math.h"

namespace cdc {

struct BasicCullingVolume;

class IDrawable {
public:
	// 15 methods
	virtual void GetBoundingVolume(BasicCullingVolume*) = 0; // 4
	virtual void draw(Matrix *, float) = 0; // 1C
};

}
