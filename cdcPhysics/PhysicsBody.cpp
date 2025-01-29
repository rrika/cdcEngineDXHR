#include "PhysicsBody.h"
#include "cdcMath/Math.h"

namespace cdc {

bool InvertSymmetric3x3(Matrix *B, MatrixArg A) { // line 24
	// TODO
	return false;
}

void PhysicsBodyImpl::Clear() { // line 307
	// TODO
}

Quat PhysicsBodyImpl::ComputeQuaternionUpdate(float dt) { // line 422
	// TODO
	return q;
}

void PhysicsBodyImpl::UpdatePosition(float dt) { // line 465
	if (flags & 1)
		return;
	auto x1 = x + v * dt;
	auto q1 = ComputeQuaternionUpdate(dt);
	if ((flags & 0x20) == 0) {
		// TODO: check tunnel risk
	}
	if ((flags & 0x20) == 1) {
		// TODO: check tunnel
	}
	x = {x1};
	q = {q1};
	// TODO: assign to all geometries
}

}
