#include "Instance.h"
#include "InstanceManager.h"

std::vector<Instance*> InstanceManager::s_instances;

Instance *InstanceManager::CreateInstance1() {
	// TODO
	Instance *instance = new Instance;
	instance->flags |= 2;
	s_instances.push_back(instance);
	return instance;
}

Instance *InstanceManager::CreateInstance2() {
	// TODO
	Instance *instance = new Instance;
	s_instances.push_back(instance);
	return instance;
}
