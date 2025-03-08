#pragma once
#include "Geom.h"
#include "cdcMath/VectorInlines.h"

namespace cdc {

class GeomDisc : public Geom {
	float m_radius; // 30

public:
	GeomDisc(Vector3Arg x, QuatArg q, float radius) :
		Geom(q, x),
		m_radius(radius)
	{}

	// GeomDisc(Vector3Arg x, Vector3Arg n, float radius) :
	// 	Geom(Quat::Build(n, {0.f, 0.f, 1.f}), x),
	// 	m_radius(radius)
	// {}

	Vector3 GetLocalSupportPoint(Vector3Arg n2) const override {
		Vector3 n = n2;
		n.z = 0.f;
		if (n.CanNormalize())
			return n * m_radius;
		return {};
	}
};

}
