#include "ContactJoint.h"

namespace cdc {

void ContactJoint::Set( // line 25
	PhysicsBodyImpl *b1, PhysicsBodyImpl *b2, Contact const *contacts, uint32_t numPoints,
	Vector3Arg normal_, float mu, float timeStep
) {
	if (b1) {
		// TODO
	} else {
		// TODO
	}
	for (uint32_t i=0; i < numPoints; i++) {
		geometry1[i] = contact->geometry1;
		geometry2[i] = contact->geometry2;
	}
	if (friction == 0.f) {
		byteF8 = numPoints;
		return;
	}
	// TODO
	if (b1) {
		// TODO
	} else if (byteF9 & 0x20) {
		// TODO
	}
}

void ContactJoint::ComputeJacobian( // line 192
	Vector3 *J, float *cPos, float *cVel, float *lo, float *hi,
	int32_t row0, JointParams const& params
) {
	uint32_t count = byteF9 & 15;
	float gravL = fsqrt(params.gravity.LenSquared());
	if (body1 == nullptr) {

	}
	while (count--) {
		cPos[…] = …;
		lo[…] = …;
		hi[…] = …;
	}
	if (… < friction) {

	}
}

}
