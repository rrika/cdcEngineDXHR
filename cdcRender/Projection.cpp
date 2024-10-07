#include <cmath>
#include "Projection.h"

namespace cdc {

Matrix BuildPerspectiveLH(float fov, float aspect, float nearz, float farz) {
	float halfFov = 0.5f * fov;
	float ry = tanf(halfFov);
	float rx = ry * aspect;
	float rangez = (farz - nearz);
	return {
		1.0f / rx, 0.0f,      0.0f,                   0.0f,
		0.0f,      1.0f / ry, 0.0f,                   0.0f,
		0.0f,      0.0f,      farz / rangez,          1.0f,
		0.0f,      0.0f,      -nearz * farz / rangez, 0.0f
	};
}

Matrix BuildOrthographicLH(float width, float height, float nearz, float farz) {
	float z = 1.0f / (farz - nearz);
	return {
		2.0f / width, 0.0f,          0.0f,      0.0f,
		0.0f,         2.0f / height, 0.0f,      0.0f,
		0.0f,         0.0f,          z,         0.0f,
		0.0f,         0.0f,          z * nearz, 0.0f
	};
}

}
