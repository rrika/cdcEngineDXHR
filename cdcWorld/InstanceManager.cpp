#include "Instance.h"
#include "InstanceManager.h"

Instance *InstanceManager::CreateInstance1() {
	// TODO
	Instance *instance = new Instance;
	instance->flags |= 2;
	return instance;
}

Instance *InstanceManager::CreateInstance2() {
	// TODO
	Instance *instance = new Instance;
	return instance;
}
