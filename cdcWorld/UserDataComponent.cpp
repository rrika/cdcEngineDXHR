#include "UserDataComponent.h"
#include "cdcWorld/Instance.h"

UserDataComponent *UserDataComponent::CreateIfNeeded(Instance *instance) {
	if (!instance->userDataComponent) {
		auto *udc = new UserDataComponent(instance);
		instance->userDataComponent = udc;
	}
	return instance->userDataComponent;
}
