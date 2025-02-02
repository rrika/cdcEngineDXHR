#pragma once
#include "collideMB.h"

namespace cdc {

class MultibodySystemImpl;

CollideCode CollideSweptSphereAndSphere(
	float& lambda,
	Vector3 *normal,
	MSphere const& sphere,
	Vector3Arg d,
	MSphere const& other);

void CollidePhantoms(MultibodySystemImpl *mb);

}
