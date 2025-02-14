#pragma once
#include "cdcMath/Math.h"

namespace cdc {

struct JointParams;

struct ContactJoint {

	void Set(PhysicsBodyImpl *b1, PhysicsBodyImpl *b2, Contact const *contacts, uint32_t numPoints, Vector3Arg normal_, float mu, float timeStep);
	void ComputeJacobian(Vector3 *J, float *cPos, float *cVel, float *lo, float *hi, int32_t row0, JointParams const& params);
};


}