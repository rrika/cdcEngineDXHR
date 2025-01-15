#pragma once
#include "Geom.h"

namespace cdc {

class GeomSphere : public Geom {
public:
	float radius;

	GeomSphere(Vector3Arg x, float radius) :
		Geom(x), radius(radius)
	{}
};

}
