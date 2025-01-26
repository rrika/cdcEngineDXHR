#pragma once
#include "Multibody.h"

namespace cdc {

class PhysicsBodyImpl : public PhysicsBody { // line 28
public:
	char name[32];
	MultibodySystemImpl *multibody; // 130
	PhysicsBodyImpl *prev; // 134
	PhysicsBodyImpl *next; // 138
	//
	void Clear() {
		// TODO
	}
};

inline void PhysicsBody::SetName(const char *name) {
	auto *b = static_cast<PhysicsBodyImpl*>(this);
	strncpy(b->name, name, 32);
	name[31] = '\0';
}


}
