#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class ICameraManager {
public:
	virtual void update() = 0; // 8

	virtual void pushFreeCamera() = 0; // 24
	virtual void popFreeCamera() = 0; // 28
	virtual void pushModelViewCamera() = 0; // 2C
	virtual void popModelViewCamera() = 0; // 30
	virtual void pushCinematicCamera() = 0; // 34
	virtual void popCinematicCamera() = 0; // 38
	virtual void pushObjectDebugCamera() = 0; // 3C
	virtual void popObjectDebugCamera() = 0; // 40
	virtual void pushOrbitDebugCamera() = 0; // 44
	virtual void popOrbitDebugCamera() = 0; // 48

	virtual bool isPlayerCamera() = 0; // 4C
	virtual bool isFreeCamera() = 0; // 50
	virtual bool isModelViewCamera() = 0; // 54
	virtual bool isCinematicCamera() = 0; // 58
	virtual bool isTransitionCamera() = 0; // 5C

	virtual float getNearPlane() = 0; // C0

	virtual Matrix *getActiveCameraMatrix() = 0; // D4
	virtual Matrix *getMatrix() = 0; // D8
};

}
