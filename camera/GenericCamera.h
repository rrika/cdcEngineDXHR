#pragma once
#include "cdcMath/Math.h"
#include "ICamera.h"

class GenericCamera : public cdc::ICamera {
public:
	cdc::Matrix matrix; // 80

	cdc::Matrix *getMatrix() override { return &matrix; } // 48
};
