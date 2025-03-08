#pragma once
#include "Geom.h"

namespace cdc {

class GeomRect : public Geom {
	Vector2 m_extents;

public:
	GeomRect(Vector2Arg extents) : m_extents(extents) {}

	Vector3 GetLocalSupportPoint(Vector3Arg n) const override {
		return {
			n.x < 0.f ? -m_extents.x : m_extents.x,
			n.y < 0.f ? -m_extents.y : m_extents.y,
			0.f
		};
	}
};

}
