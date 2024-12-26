#pragma once
#include "ICamera.h"

class TransitionCamera : public cdc::ICamera {
	ICamera *a; // B4
	ICamera *b; // B8
public:
	void enable(ICamera *prevCamera) override { /* TODO */ }; // 2C
	void disable(ICamera *nextCamera) override { /* TODO */ }; // 30
	Type getType() override { return kTransition; } // 38
	cdc::Matrix *getMatrix() override { /* TODO */ return nullptr; } // 48
	float getNearPlane() override { // 4C
		return b ? b->getNearPlane() : 40.f;
	}
};
