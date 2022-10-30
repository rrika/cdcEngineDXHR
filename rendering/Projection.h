#pragma once
#include "cdcMath/Math.h"

namespace cdc {

Matrix BuildPerspectiveLH(float fov, float aspect, float nearz, float farz);
// BuildPerspectiveLHOffCenter
// BuildPerspectiveLHOffset
// BuildPerspectiveLHLinearZ
Matrix BuildOrthographicLH(float width, float height, float nearz, float farz);
// BuildOrthographicLHOffCenter

}
