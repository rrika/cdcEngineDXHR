#pragma once
#include "Geom.h"

namespace cdc {

class GeomPoint : public Geom {
public:
	GeomPoint(Vector3Arg pos) : Geom(pos) {}
};

}
