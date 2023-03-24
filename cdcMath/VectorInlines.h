#pragma once
#include "Math.h"

namespace cdc {

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

}
