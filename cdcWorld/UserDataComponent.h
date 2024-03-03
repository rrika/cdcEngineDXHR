#pragma once
#include "cdcWorld/InstanceComponent.h"

// namespaced as cdc::InstanceUserData in Tomb Raider
struct InstanceUserData {
	// empty
};

// namespaced as cdc::UserDataComponent in Tomb Raider
class UserDataComponent : public InstanceComponent {
public:
	Instance *instance;
	InstanceUserData *userData;

	UserDataComponent(Instance *i) : instance(i), userData(nullptr) {}
	static UserDataComponent *CreateIfNeeded(Instance*);
};
