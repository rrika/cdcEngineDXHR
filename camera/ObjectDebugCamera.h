#pragma once
#include "FreeCamera.h"

class ObjectDebugCamera : public FreeCamera {
public:
	void enable(ICamera *prevCamera) override { /* TODO */ }; // 2C
	void disable(ICamera *nextCamera) override { /* TODO */ }; // 30
	// inherit getType, returning kFree
	cdc::Matrix *getMatrix() override { /* TODO */ return nullptr; } // 48
};
