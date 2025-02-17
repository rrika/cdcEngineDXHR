#pragma once
#include "Geom.h"

namespace cdc {

class GeomDisc : public Geom {
public:
	float radius;
	Vector3 GetLocalSupportPoint(Vector3Arg n) const override { /* TODO */ }
};

}
