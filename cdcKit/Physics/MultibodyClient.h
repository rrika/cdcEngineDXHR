#pragma once
#include "cdcPhysics/MultibodySystem.h"

class MultibodyClient : public cdc::MultibodySystemImpl { // line 322
public:
	bool CreateBodyFromModel(Instance*, int32_t, dtp::rigidbody const*, cdc::Matrix*, float) override;
};

extern MultibodyClient *gMultibodySystem;
