#pragma once
#include "Geom.h"

namespace cdc {

class GeomWrap : public Geom {
	Vector3 m_x1 = {0.f, 0.f, 0.f};
	Vector3 m_x2 = {0.f, 0.f, 0.f};
	Vector3 m_center;
	Geom *m_g1;
	Geom *m_g2;

public:
	GeomWrap(Geom *g1, Geom *g2) : m_g1(g1), m_g2(g2) {
		m_center = (g1->GetGlobalCenter() + g2->GetGlobalCenter()) * 0.5f;
	}
	Vector3 GetLocalSupportPoint(Vector3Arg n) const override { /* TODO */ }
};

}
