#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class ICamera {
public:
	virtual Matrix *getMatrix() = 0; // 48
};

}
