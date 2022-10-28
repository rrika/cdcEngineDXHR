#pragma once
#include "Geom.h"

namespace cdc {

class GeomSegment : public Geom {
public:
	Vector3 p1;
	Vector3 p2;
};

}
