#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "cdcWorld/InstanceComponent.h"

class Instance;

namespace cdc {

struct Matrix;

class TransformComponent : public InstanceComponent {
public:
	// Instance *m_instance = nullptr; // 4
	Vector3 m_scale {1.0f, 1.0f, 1.0f, 1.0f}; // 10
	Matrix *m_matrix = nullptr; // 20
		// allocated by MeshComponent::SetModel (InstanceDrawable.h)
		// written by G2Instance_SetTransformsToIdentity
		// read by UpdateInstances
	Matrix *matrixBuffer = nullptr; // 28
	int8_t m_rootMatrix = 0; // 31
	uint8_t m_flags = 0; // C3, read by MeshComponent::SetModel
public:
	TransformComponent() = default;

	void SetNotAnimated(bool x) { m_flags = (m_flags & ~4) | (x ? 4 : 0); }
	bool GetNotAnimated() { return m_flags & 4; }
};

}
