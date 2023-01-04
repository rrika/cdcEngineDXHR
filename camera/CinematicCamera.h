#pragma once
#include "GenericCamera.h"

class CinematicCamera : public GenericCamera {
	Type getType() override { return kCinematic; } // 38
};
