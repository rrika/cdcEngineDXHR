#include <cmath>
#include "MathUtil.h"

namespace cdc {
namespace MathUtil {

static const float piSixth = 0.52359879f;

void QuatLogToMatrix(Matrix *matrix, const Quat *quat) {

	float i = quat->vec128[0];
	float j = quat->vec128[1];
	float k = quat->vec128[2];
	float i2 = i * i;
	float j2 = j * j;
	float k2 = k * k;
	float ijk_len_sq = j2 + i2 + k2;

	if (ijk_len_sq > 0.000001f) {

		float t, u;

		if (piSixth * piSixth <= ijk_len_sq) {
			float ijk_len = sqrtf(ijk_len_sq);
			float angle = ijk_len * 0.5;
			float sin_angle = sinf(angle);
			float cos_angle = cosf(angle);
			t = sin_angle / ijk_len;
			u = cos_angle;
		} else {
			t = 0.5 - (0.02083333395421505 - ijk_len_sq / 3840.0) * ijk_len_sq;
			float v21 = ijk_len_sq * t * t;
			u = 1.0 - v21 * ((0.0625 * v21 + 0.125) * v21 + 0.5);
		}

		float s = t * t;                              // s = t*t
		float two_s = s * 2.0;
		float two_s_r = t * (2.0 * u);                // 2sr = 2ttr = 2tu; u = tr; r = u/t

		float two_s_i2 = two_s * i2;
		float two_s_j2 = two_s * j2;
		float two_s_k2 = two_s * k2;

		float two_s_ij = two_s * i * j;
		float two_s_ik = two_s * i * k;
		float two_s_jk = two_s * j * k;

		float two_s_ir = two_s_r * i;
		float two_s_jr = two_s_r * j;
		float two_s_kr = two_s_r * k;

		matrix->m[0][0] = 1.0f - (two_s_k2 + two_s_j2);
		matrix->m[0][1] = two_s_kr + two_s_ij;
		matrix->m[0][2] = two_s_ik - two_s_jr;
		matrix->m[0][3] = 0.0f;
		matrix->m[1][0] = two_s_ij - two_s_kr;
		matrix->m[1][1] = 1.0f - (two_s_k2 + two_s_i2);
		matrix->m[1][2] = two_s_ir + two_s_jk;
		matrix->m[1][3] = 0.0f;
		matrix->m[2][0] = two_s_ik + two_s_jr;
		matrix->m[2][1] = two_s_jk - two_s_ir;
		matrix->m[2][2] = 1.0f - (two_s_i2 + two_s_j2);
		matrix->m[2][3] = 0.0f;
		// matrix->m[3][0] = 0.0f;
		// matrix->m[3][1] = 0.0f;
		// matrix->m[3][2] = 0.0f;
		matrix->m[3][3] = 1.0f;
	}
	else
		*matrix = identity4x4;
}

}
}

