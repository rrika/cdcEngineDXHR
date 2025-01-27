#pragma once
#include "Math.h"

namespace cdc {

inline Matrix& Matrix::operator*=(float t) { // line 130
	return *this = *this * t;
}

inline Vector3 operator*(MatrixArg m, Vector3Arg v) { // line 195
	return {
		m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0],
		m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1],
		m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2],
		m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3]
	};
}

inline Vector4 operator*(MatrixArg m, Vector4Arg v) { // line 196
	return {
		m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w,
		m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w,
		m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w,
		m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] * v.w
	};
}

inline Matrix& Matrix::operator+=(MatrixArg b) { // line 221
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			m[i][j] += b.m[i][j];
	return *this;
}

}
