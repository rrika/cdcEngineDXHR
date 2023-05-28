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
	int8_t m_rootMatrix = 0; // 31
	uint8_t m_flags = 0; // C3, read by MeshComponent::SetModel
public:
	TransformComponent() = default;
};

}
