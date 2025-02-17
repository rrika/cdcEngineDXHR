#pragma once
#include "Geom.h"

namespace cdc {

class GeomTriangle : public Geom {
public:
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
	Vector3 GetLocalSupportPoint(Vector3Arg n) const override { /* TODO */ }
};

}
