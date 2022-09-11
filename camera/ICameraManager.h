#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class ICameraManager {
public:
	virtual void update() = 0; // 8
	virtual Matrix *getActiveCameraMatrix() = 0; // D4
	virtual Matrix *getMatrix() = 0; // D8
};

}
