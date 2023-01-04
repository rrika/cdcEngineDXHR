#pragma once
#include "GenericCamera.h"

class OrbitDebugCamera : public GenericCamera {
public:
	void enable(ICamera *prevCamera) override { /* TODO */ }; // 2C
	void disable(ICamera *nextCamera) override { /* TODO */ }; // 30
	// inherit getType, returning kGeneric
	cdc::Matrix *getMatrix() override { /* TODO */ return nullptr; } // 48
};
