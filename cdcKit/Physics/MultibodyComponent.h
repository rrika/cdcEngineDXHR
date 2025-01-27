#pragma once
#include <vector>
#include "cdcMath/Math.h"
#include "cdcWorld/InstanceComponent.h"

namespace dtp { struct PhysicalComponent; }

class Instance;

namespace cdc {

class PhysicsBody;

struct MultibodyEvent { // line 34
	// TODO
};

class MultibodyComponent : public InstanceComponent { // called cdc::RigidbodyComponent in TR
public:
	Vector3 m_centerOfMass; // 10
	Instance *m_instance; // 20
	std::vector<MultibodyEvent> m_events; // 24
	uint32_t dword30;
	PhysicsBody *m_body; // 34
	dtp::PhysicalComponent *m_pRigidBodyDTP; // 38
	uint32_t m_flags; // 3C, MultibodyComponentFlags
};

}
