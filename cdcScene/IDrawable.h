#pragma once
#include "cdcMath/Math.h"

namespace cdc {

struct BasicCullingVolume;

class IDrawable {
public:
	// 15 methods
	virtual void GetBoundingVolume(BasicCullingVolume*) = 0; // 4
	virtual void draw(Matrix *, float) = 0; // 1C
	virtual bool GetBoundingSphere(Vector *pCenter, float *pRadius) { return false; } // 2C
	virtual bool GetBoundingBox(Vector *pMin, Vector *pMax) { return false; } // 30
	virtual ~IDrawable() = default; // 38
};

}
