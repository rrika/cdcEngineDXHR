#pragma once
#include <cstring>
#include "cdcMath/Math.h"
#include "MassProperties.h"
#include "Multibody.h"

namespace cdc {

bool InvertSymmetric3x3(cdc::Matrix*, cdc::MatrixArg);

class MultibodySystemImpl;

class PhysicsBodyImpl : public PhysicsBody { // line 28
public:
	Matrix I; // 0
		// this is a symmetric 3x3 matrix stored in 4x4 layout
		// only the following six elements are valid
		// [0][0]
		// [1][0] [1][1]
		// [2][0] [2][1] [2][2]
	Matrix invI;
	Vector3 x;     // 80, position
	Quat q;        // 90, rotation
	Vector3 v;     // A0, velocity
	Vector3 omega; // B0, angular velocity
	Vector3 F;     // C0, force
	Vector3 T;     // D0, torque
	Vector3 Fc;    // E0, force c?
	Vector3 Tc;    // F0, torque c?
	// SleepProperties {
	float linearThreshold; // 100
	float angularThreshold; // 104
	float sleepDelay; // 108
	bool autoSleep; // 10C
	// } sleep;
	char name[32]; // 110
	MultibodySystemImpl *multibody; // 130
	PhysicsBodyImpl *prev; // 134
	PhysicsBodyImpl *next; // 138
	void *clientData; // 148
	float mass; // 14C
	float invMass; // 150
	float linearDamping; // 154
	float angularDamping; // 158
  	uint32_t flags; // 15C
  	float sleepTimer; // 160
  	uint32_t index; // 164
	//
	void Clear();
	Quat ComputeQuaternionUpdate(float dt);
	void UpdatePosition(float dt);
};

inline void PhysicsBody::SetName(const char *name) {
	auto *b = static_cast<PhysicsBodyImpl*>(this);
	strncpy(b->name, name, 32);
	b->name[31] = '\0';
}

inline void PhysicsBody::SetMassProperties(MassProperties const& props) {
	auto *b = static_cast<PhysicsBodyImpl*>(this);
	if (props.CheckValidity()) {
		b->mass = props.mass;
		b->I = props.I;
	} else {
		MULTIBODY_PrintWarning("WARNING: Body %s has bad mass properties\n", b->name);
		b->mass = 666.f;
		b->I = 66666.f * identity4x4;
		b->flags |= 4;
	}
	b->invMass = 1 / b->mass;
	InvertSymmetric3x3(&b->invI, b->I);
}

inline void PhysicsBody::SetClientData(void *clientData) {
	auto *b = static_cast<PhysicsBodyImpl*>(this);
	b->clientData = clientData;
}

}
