#pragma once
#include "../math/Math.h"

namespace cdc {

class Geom {
public:
	virtual ~Geom() = default;
	Vector3 position; // 10
	Quat rotation; // 20
};

}
