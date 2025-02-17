#pragma once
#include "Geom.h"

namespace cdc {

class GeomSegment : public Geom {
public:
	Vector3 m_point1;
	Vector3 m_point2;

	GeomSegment(Vector3Arg x1, Vector3Arg x2) :
		m_point1(x1), m_point2(x2)
	{}

	Vector3 GetLocalSupportPoint(Vector3Arg n) const override { /* TODO */ }
};

}
