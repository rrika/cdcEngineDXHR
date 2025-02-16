#pragma once
#include "Multibody.h"
#include "cdcMath/Math.h"

namespace cdc {

struct JointParams { // line 39
	Vector3 gravity; // 0
	float dt; // 10, see also TimeStepIslandParams::dt
	float thickness; // 14
	float threshold; // 18
	float invBeta; // 1C
};

class PhysicsJointImpl : public PhysicsJoint {
	//
};

}
