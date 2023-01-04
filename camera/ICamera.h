#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class ICamera {
public:
	enum Type {
		kGeneric = 0, // including OrbitDebugCamera
		kPlayer = 1,
		kTransition = 2,
		kAnchored = 3,
		kFree = 4, // including ObjectDebugCamera
		kModelView = 5,
		kCinematic = 6,
		kAutoSpline = 7
	};

	virtual void enable(ICamera *prevCamera) = 0; // 2C
	virtual void disable(ICamera *nextCamera) = 0; // 30
	virtual Type getType() = 0; // 38
	virtual Matrix *getMatrix() = 0; // 48
};

}
