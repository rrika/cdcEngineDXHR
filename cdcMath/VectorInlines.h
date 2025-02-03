#pragma once
#include <cmath>
#include "Math.h"

namespace cdc {

inline void Vector::SetZero() { // line 88
	x = y = z = w = 0.f;
}

inline Vector& Vector::operator*=(float valB) { // line 120
	x *= valB;
	y *= valB;
	z *= valB;
	w *= valB;
	return *this;
}


inline float Vector3::LenSquared() const { // line 257
	return x*x + y*y + z*z;
}

inline Vector3 operator+(Vector3Arg vecA, Vector3Arg vecB) { // line 363
	return {
		vecA.x + vecB.x,
		vecA.y + vecB.y,
		vecA.z + vecB.z,
		0.0f,
	};
}

inline Vector3 operator-(Vector3Arg vecA, Vector3Arg vecB) { // line 375
	return {
		vecA.x - vecB.x,
		vecA.y - vecB.y,
		vecA.z - vecB.z,
		0.0f,
	};
}

inline Vector3 operator*(Vector3Arg vecA, float valB) { // line 388
	return {
		vecA.x * valB,
		vecA.y * valB,
		vecA.z * valB,
		0.0f,
	};
}

inline float Dot3(VectorArg vecA, VectorArg vecB) { // line 575
	return
		vecA.x * vecB.x +
		vecA.y * vecB.y +
		vecA.z * vecB.z;
}

inline Vector Cross3(VectorArg vecA, VectorArg vecB) { // line 637
	const float *a = vecA.vec128;
	const float *b = vecB.vec128;
	return {
		a[1] * b[2] - a[2] * b[1],
		a[2] * b[0] - a[0] * b[2],
		a[0] * b[1] - a[1] * b[0],
		a[3] * b[3] - a[3] * b[3]
	};
}

inline Vector CompMin4(VectorArg vecA, VectorArg vecB) { // line 711
	return {
		fminf(vecA.x, vecB.x),
		fminf(vecA.y, vecB.y),
		fminf(vecA.z, vecB.z),
		fminf(vecA.w, vecB.w)
	};
}

inline Vector CompMax4(VectorArg vecA, VectorArg vecB) { // line 723
	return {
		fmaxf(vecA.x, vecB.x),
		fmaxf(vecA.y, vecB.y),
		fmaxf(vecA.z, vecB.z),
		fmaxf(vecA.w, vecB.w)
	};
}

inline bool Vector::IsZero2() const { // line 835
	float limit = 1.f/0x4000;
	if (-limit < x && x < limit)
		if (-limit < y && y < limit)
			return true;
	return false;
}

inline bool Vector::IsZero3() const { // line 845
	float limit = 1.f/0x4000;
	if (-limit < x && x < limit)
		if (-limit < y && y < limit)
			if (-limit < z && z < limit)
				return true;
	return false;
}

inline bool Vector::IsZero4() const { // line 855
	float limit = 1.f/0x4000;
	if (-limit < x && x < limit)
		if (-limit < y && y < limit)
			if (-limit < z && z < limit)
				if (-limit < w && w < limit)
					return true;
	return false;
}

}
