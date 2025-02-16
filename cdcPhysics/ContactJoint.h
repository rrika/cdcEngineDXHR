#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "ContactManager/Contact.h"

namespace cdc {

class PhysicsBody;
class PhysicsBodyImpl;
class PhysicsGeometry;
struct JointParams;

struct ContactJoint {

	float friction; // 78
	float gamma; // 7C
	float lambda[8]; // A8
	PhysicsBodyImpl *body1; // C8
	PhysicsBodyImpl *body2; // CC
	PhysicsGeometry *geometry1[5]; // D0
	PhysicsGeometry *geometry2[5]; // E4
	uint8_t byteF8;
	uint8_t byteF9;

	void Set(PhysicsBodyImpl *b1, PhysicsBodyImpl *b2, Contact const *contacts, uint32_t numPoints, Vector3Arg normal_, float mu, float timeStep);
	void ComputeJacobian(Vector3 *J, float *cPos, float *cVel, float *lo, float *hi, int32_t row0, JointParams const& params);
};

}
