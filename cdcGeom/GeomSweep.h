#pragma once
#include "Geom.h"

namespace cdc {

class GeomSweep : public Geom {
public:
	Vector3 x1;
	Vector3 x2;
	Vector3 center;
	Geom *g1;
	Geom *g2;
};

}
