#pragma once
#include "ICamera.h"

class AnchoredCamera : public cdc::ICamera {
public:
	void enable(ICamera *prevCamera) override { /* TODO */ }; // 2C
	void disable(ICamera *nextCamera) override { /* TODO */ }; // 30
	Type getType() override { return kAnchored; } // 38
	cdc::Matrix *getMatrix() override { /* TODO */ return nullptr; } // 48
};
