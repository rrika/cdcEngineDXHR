#pragma once
#include "cdcPhysics/MultibodySystem.h"
#include "cdcWorld/cdcWorldTypes.h" // for HPrim

struct GeometryClientData { // line 262
	Instance *instance;
	HPrim *prim;
	// cdc::RagdollBone *bone;
	// cdc::ProxyComponent *proxy;
};

class MultibodyClient : public cdc::MultibodySystemImpl { // line 322
public:
	bool CreateBodyFromModel(Instance*, int32_t, dtp::rigidbody const*, cdc::Matrix*, float) override;
	void CalcMassPropertiesFromHModel(HModel const*, cdc::MassProperties*) override;
	void AddGeometriesToBody(cdc::PhysicsBody*, HModel const*, cdc::Vector3 const*, dtp::rigidbody const*, Instance*) override;
};

extern MultibodyClient *gMultibodySystem;
