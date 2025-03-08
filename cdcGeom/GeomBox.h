#pragma once
#include "Geom.h"

namespace cdc {

class GeomBox : public Geom {
	Vector3 m_extents; // 30

public:
	GeomBox(Vector3 extents) : m_extents(extents) {}

	Vector3 GetLocalSupportPoint(Vector3Arg n) const override {
		return {
			n.x < 0.f ? -m_extents.x : m_extents.x,
			n.y < 0.f ? -m_extents.y : m_extents.y,
			n.z < 0.f ? -m_extents.z : m_extents.z
		};
	}
};

}
