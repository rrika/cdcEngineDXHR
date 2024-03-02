#include "InstncG2.h"
#include "cdcMath/Math.h"
#include "cdcWorld/Instance.h"

namespace cdc {

// HACK
void G2Instance_SetTransformsToIdentity(Instance *instance) {

	if (instance->GetNumModels() == 0)
		return;

	auto& position = instance->position;
	auto& rotation = instance->rotation;
	auto& scale = instance->GetTransformComponent().m_scale;

	cdc::Matrix instanceMatrix = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		position.x, position.y, position.z, 1
	};
	cdc::Matrix rotationMatrix; rotationMatrix.Build_XYZOrder(rotation.vec128);
	instanceMatrix = instanceMatrix * rotationMatrix;

	auto& transformComponent = instance->GetTransformComponent();
	if (!transformComponent.GetNotAnimated())
		transformComponent.m_matrix[-1] = instanceMatrix;

	transformComponent.m_matrix[0] = instanceMatrix;

	// these matrices are read by CalcSkeletonMatrices via InstanceDrawable::PrepareMatrixState
}

}
