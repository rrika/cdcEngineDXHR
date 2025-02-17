#pragma once
#include "Geom.h"

namespace cdc {

class GeomSphere : public Geom {
public:
	float radius;

	GeomSphere(Vector3Arg x, float radius) :
		Geom(x), radius(radius)
	{}

	Vector3 GetLocalSupportPoint(Vector3Arg n) const override {
		return n * radius;
	}
	Vector3 GetGlobalSupportPoint(Vector3Arg n) const override {
		return n * radius;
	}
};

}
