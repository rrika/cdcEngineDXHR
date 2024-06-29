#pragma once
#include "cdcMath/Math.h"
#include "cdcWorld/InstanceComponent.h"

namespace cdc {

// In DXHR this was a game specific component (not in cdc namespace, not in cdcObjects folder)
// but since it is referenced from class Instance it makes more sense to have it engine side.
// Incidentally that's exactly what was done in Tomb Raider.

class BallisticComponent : public InstanceComponent {
public:
	Vector m_vel {0.f, 0.f, 0.f, 0.f};
	Vector m_accl {0.f, 0.f, 0.f, 0.f};
	Vector m_maxVel {0.f, 0.f, 0.f, 0.f};
};

}
