#include "Instance.h"
#include "InstanceManager.h"

std::unordered_map<int, Instance*> InstanceManager::gInstanceIntroUniqueIDHashMap;
std::vector<Instance*> InstanceManager::s_instances;
Instance *InstanceManager::s_chain = nullptr;

Instance *InstanceManager::CreateInstance1() {
	// TODO
	Instance *instance = new Instance;
	instance->next = s_chain;
	s_chain = instance;
	instance->flags |= 2;
	s_instances.push_back(instance);
	return instance;
}

Instance *InstanceManager::CreateInstance2() {
	// TODO
	Instance *instance = new Instance;
	instance->next = s_chain;
	s_chain = instance;
	s_instances.push_back(instance);
	return instance;
}
