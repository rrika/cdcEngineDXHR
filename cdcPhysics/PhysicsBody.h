#pragma once
#include "Multibody.h"

namespace cdc {

class PhysicsBodyImpl : public PhysicsBody {
public:
	MultibodySystemImpl *multibody; // 130
	PhysicsBodyImpl *prev; // 134
	PhysicsBodyImpl *next; // 138
	//
	void Clear() {
		// TODO
	}
};

}
