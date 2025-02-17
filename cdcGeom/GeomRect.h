#pragma once
#include "Geom.h"

namespace cdc {

class GeomRect : public Geom {
	Vector2 extents;
public:
	Vector3 GetLocalSupportPoint(Vector3Arg n) const override { /* TODO */ }
};

}
