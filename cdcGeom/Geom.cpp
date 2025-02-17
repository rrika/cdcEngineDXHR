#include "Geom.h"

namespace cdc {

Vector3 Geom::GetLocalCenter() const {
	return {0.f, 0.f, 0.f};
}

Vector3 Geom::GetGlobalSupportPoint(Vector3Arg n) const {
	Quat unrotation {-m_rotation.x, -m_rotation.y, -m_rotation.z, m_rotation.w};
	return {m_position + QuatRotate(m_rotation,
		GetLocalSupportPoint(QuatRotate(unrotation, n)))};
}

Vector3 Geom::GetGlobalCenter() const {
	return m_position;
}

void Geom::GetBBox(Vector3& mins, Vector3& maxs) const {
	mins.x = GetGlobalSupportPoint({-1.f, 0.f, 0.f}).x;
	mins.y = GetGlobalSupportPoint({0.f, -1.f, 0.f}).y;
	mins.z = GetGlobalSupportPoint({0.f, 0.f, -1.f}).z;
	maxs.x = GetGlobalSupportPoint({1.f, 0.f, 0.f}).x;
	maxs.y = GetGlobalSupportPoint({0.f, 1.f, 0.f}).y;
	maxs.z = GetGlobalSupportPoint({0.f, 0.f, 1.f}).z;
}

}
