#pragma once
#include "Geom.h"
#include "cdcMath/VectorInlines.h"

namespace cdc {

class GeomSegment : public Geom {
	Vector3 m_point1;
	Vector3 m_point2;

public:
	GeomSegment(Vector3Arg x1, Vector3Arg x2) :
		m_point1(x1), m_point2(x2)
	{}

	Vector3 GetLocalSupportPoint(Vector3Arg n) const override {
		if (Dot3(m_point2 - m_point1, n) > 0.f)
			return m_point2;
		else
			return m_point1;
	}
};

}
