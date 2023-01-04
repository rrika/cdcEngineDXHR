#pragma once
#include "cdcMath/Math.h"
#include "ICamera.h"

class GenericCamera : public cdc::ICamera {
public:
	cdc::Matrix matrix; // 80

	void enable(ICamera *prevCamera) override { /* TODO */ }; // 2C
	void disable(ICamera *nextCamera) override { /* TODO */ }; // 30
	Type getType() override { return kGeneric; } // 38
	cdc::Matrix *getMatrix() override { return &matrix; } // 48
};
