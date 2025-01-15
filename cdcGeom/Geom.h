#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class Geom {
public:
	Vector3 position; // 10
	Quat rotation; // 20

	virtual ~Geom() = default;
	Geom() : position{0, 0, 0, 0}, rotation{0, 0, 0, 1} {}
	Geom(Vector3Arg pos) : position(pos), rotation{0, 0, 0, 1} {}
};

}
