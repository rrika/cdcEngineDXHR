#include <cmath>
#include "cdcMath/VectorInlines.h"
#include "ContactJoint.h"
#include "PhysicsBody.h"
#include "PhysicsJoint.h"

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
		geometry1[i] = contacts[0].geometry1;
		geometry2[i] = contacts[0].geometry2;
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
		// TODO
	}
	uint32_t row = row0;
	while (count--) {
		cPos[row] = 0.f; // TODO
		lo[row] = 0.f; // TODO
		hi[row] = 0.f; // TODO
		row0++;
	}
	if (0.f /*TODO*/ < friction) {
		// TODO
	}
}

}
