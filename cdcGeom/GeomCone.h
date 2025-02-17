#pragma once
#include "Geom.h"

namespace cdc {

class GeomCone : public Geom {
public:
	GeomDisc disc;
	GeomPoint point;
	Vector3 GetLocalSupportPoint(Vector3Arg n) const override { /* TODO */ }
};

}
