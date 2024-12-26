#pragma once
#include "ICamera.h"

class FreeCamera : public cdc::ICamera {
public:
	void enable(ICamera *prevCamera) override { /* TODO */ }; // 2C
	void disable(ICamera *nextCamera) override { /* TODO */ }; // 30
	Type getType() override { return kFree; } // 38
	cdc::Matrix *getMatrix() override { /* TODO */ return nullptr; } // 48
	float getNearPlane() override { // 4C
		return 1.f; // set to 40.f by CameraManager regardless
	}
};
