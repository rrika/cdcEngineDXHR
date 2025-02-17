#pragma once
#include "Geom.h"

namespace cdc {

class GeomPoint : public Geom {
public:
	GeomPoint(Vector3Arg pos) : Geom(pos) {}
	Vector3 GetLocalSupportPoint(Vector3Arg n) const override { /* TODO */ }
};

}
