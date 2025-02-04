#pragma once
#include "cdcMath/VectorInlines.h"
#include "collideMB.h"

namespace cdc {

inline float clamp(float a, float min, float max) {
	if (a < min) return min;
	if (a > max) return max;
	return a;
}

inline float SegmentToSegmentDistance( // line 66
	Vector3& cp1,
	Vector3& cp2,
	Vector3Arg s1,
	Vector3Arg t1,
	Vector3Arg s2,
	Vector3Arg t2,
	float *lambda1,
	float *lambda2
) {
	constexpr float kEpsilon = 1e-7f;
	Vector3 d1 = t1 - s1;
	Vector3 d2 = t2 - s2;
	Vector3 c = s1 - s2;
	float d11 = d1.LenSquared();
	float d22 = d2.LenSquared();
	float lam1, lam2;

	if (d11 >= kEpsilon && d22 >= kEpsilon) {	

		float d12 = Dot3(d1, d2);
		float cd1 = Dot3(c, d1);
		float cd2 = Dot3(c, d2);
		float denom = d11 * d22 - d12 * d12;
		if (denom >= kEpsilon) {
			lam1 = (d12 * cd2 - d22 * cd1) / denom;
			lam1 = clamp(lam1, 0.0f, 1.0f);
		} else
			lam1 = 0.0f;
		
		lam2 = (d12 * lam1 + cd2) / d22;
		if (lam2 < 0.f) {
			lam2 = 0.f;
			lam1 = clamp(-cd1 / d11, 0.f, 1.f);
		} else if (lam2 > 1.f) {
			lam2 = 1.f;
			lam1 = clamp((d12 - cd1) / d11, 0.f, 1.f);
		}

	} else if (d11 >= kEpsilon) {
		lam1 = clamp(-Dot3(c, d1) / d11, 0.0f, 1.0f);
		lam2 = 0.0f;

	} else if (d22 >= kEpsilon) {
		lam1 = 0.0f;
		lam2 = clamp(Dot3(c, d2) / d22, 0.0f, 1.0f);

	} else {
		lam1 = 0.0f;
		lam2 = 0.0f;
	}

	if (lambda1) *lambda1 = lam1;
	if (lambda2) *lambda2 = lam2;

	cp1 = s1 + d1 * lam1;
	cp2 = s2 + d2 * lam2;

	c = cp1 - cp2;

	return c.LenSquared();
}

inline bool TestAlignedBoxAndAlignedBox( // line 77
	Vector3Arg aMin,
	Vector3Arg aMax,
	Vector3Arg bMin,
	Vector3Arg bMax
) {
	return
		aMax.x > bMin.x &&
		aMax.y > bMin.y &&
		aMax.z > bMin.z &&
		aMin.x < bMax.x &&
		aMin.y < bMax.y &&
		aMin.z < bMax.z;
}

inline float PointToSegmentDistance( // line 136
	float *lambda,
	Vector3 *closestPoint,
	Vector3Arg point,
	Vector3Arg s,
	Vector3Arg t
) {
	Vector3 cp = s;
	float l = 0.f;

	Vector3 d = t - s;
	float dSquared = d.LenSquared();

	if (dSquared != 0.f) {
		l = Dot3(point - s, d) / dSquared;
		l = clamp(l, 0.f, 1.f);
		cp = s + d * l;
	}

	if (lambda) *lambda = l;
	if (closestPoint) *closestPoint = cp;

	return (point - cp).LenSquared();
}

inline BBox CalcXformedBBox(BBox const& A, Matrix const& M) { // line 163
	// TODO
	return A;
}

}
