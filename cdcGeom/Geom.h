#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class Geom {
public:
	Vector3 m_position; // 10
	Quat m_rotation; // 20

	Geom() : m_position{0, 0, 0}, m_rotation{0, 0, 0, 1} {}
	Geom(Vector3Arg pos) : m_position(pos), m_rotation{0, 0, 0, 1} {}

	virtual ~Geom() = default; // 0
	virtual Vector3 GetLocalSupportPoint(Vector3Arg n) const = 0; // 4
	virtual Vector3 GetLocalCenter() const; // 8
	virtual Vector3 GetGlobalSupportPoint(Vector3Arg n) const; // C
	virtual Vector3 GetGlobalCenter() const; // 10
	virtual void GetBBox(Vector3&, Vector3&) const; // 14
};

}
