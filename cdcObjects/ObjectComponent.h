#pragma once
#include <cstdint>
#include "cdcWorld/InstanceComponent.h"

class Instance;

namespace cdc {

class ObjectComponent : public InstanceComponent {
public:
	Instance *m_instance = nullptr;
	// ObjectFamily m_family;
	uint32_t m_familyId = 0;
	// uint32_t m_flags;
public:
	ObjectComponent() = default;
	inline void SetInstance(Instance *instance) { m_instance = instance; }
	void SetProcessFunctions();
	bool InstanceInit(bool unknown);
	bool InstanceUninit(bool unknown);
};

}
