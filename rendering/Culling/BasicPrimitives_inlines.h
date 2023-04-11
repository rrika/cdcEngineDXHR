#pragma once
#include <cmath>
#include "BasicPrimitives.h"
#include "cdcMath/VectorInlines.h"
#include "cdcMath/MatrixInlines.h"

namespace cdc {

void CullingPlane::Set(Vector3Arg normal, Vector3Arg point) { // line 101
	m_planeEq = {normal};
	m_planeEq.w = Dot3(normal, point);
}

void CullingPlane::Set(Vector3Arg v1, Vector3Arg v2, Vector3Arg v3) { // line 106
	Vector3 d21 = v2-v1;
	Vector3 d31 = v3-v1;
	Vector3 n { Cross3(d21, d31) };
	n.SafeNormalize3();
	Set(n, v1);
}

void CullingPlane::Transform(Matrix const& m) { // line 147
	// HACK
	Vector4 n = m_planeEq;
	n.w = 0.0f;
	n = m * n;
	n.SafeNormalize3();
	Vector3 p = m * Vector3{m_planeEq * m_planeEq.w};
	Set(Vector3{n}, p);
}

inline void CullingSphere::Transform(Matrix const& m) { // line 225
	// TODO
}

inline void CullingBox::SetFromCenterAndExtents(Vector3Arg center, Vector3Arg extents) { // line 497
	m_exX = {extents.x, 0.0f, 0.0f};
	m_exY = {0.0f, extents.y, 0.0f};
	m_exZ = {0.0f, 0.0f, extents.z};
	m_center = {center};
	// like in SetFromMinMax
	m_center.w = 1.f;
}

inline void CullingBox::SetFromMinMax(Vector3Arg min, Vector3Arg max) { // 515
	Vector3Arg across = max-min;
	Vector3Arg center = (max+min) * 0.5;

	m_exX = {across.x, 0.f, 0.f, 0.f};
	m_exY = {0.f, across.y, 0.f, 0.f};
	m_exZ = {0.f, 0.f, across.z, 0.f};
	m_center = {center};
	// HACK (forgot why though)
	m_center.w = 1.f;
}

inline void CullingBox::Translate(Vector3Arg t) { // line 533
	m_center += t;
}

inline void CullingBox::Transform(Matrix const& m) { // line 538
	m_exX = m * m_exX;
	m_exY = m * m_exY;
	m_exZ = m * m_exZ;
	m_center = m * m_center;
}

inline CullingMinMax CullingBox::GetProjectedMinMax(Vector3Arg e) const { // 554
	// original uses Dot3Scalar, but why?
	float c = Dot3(m_center, e);
	float dx = Abs(Dot3(m_exX, e));
	float dy = Abs(Dot3(m_exY, e));
	float dz = Abs(Dot3(m_exZ, e));
	float x = dx + dy + dz;
	return {c-x, c+x};
}

inline bool InFront(CullingPlane const& a, CullingSphere const& b) { // line 1185
	return Dot3(a.m_planeEq, b.m_sphereEq) > (a.m_planeEq.w + b.m_sphereEq.w);
}

}
