#pragma once
#include "GenericCamera.h"

class PlayerCamera : public GenericCamera {
public:
	void enable(ICamera *prevCamera) override { /* TODO */ }; // 2C
	void disable(ICamera *nextCamera) override { /* TODO */ }; // 30
	Type getType() override { return kPlayer; } // 38
	cdc::Matrix *getMatrix() override { /* TODO */ return nullptr; } // 48
};
