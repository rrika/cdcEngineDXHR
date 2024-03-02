#include "InstncG2.h"
#include "cdcMath/Math.h"
#include "cdcWorld/cdcWorldTypes.h"
#include "cdcWorld/Instance.h"

namespace cdc {

static void NonAnimatedRebuild(Matrix *matrices, dtp::Model *model) { // guessed name
	uint32_t numSegments = model->GetNumSegments();
	Segment *segments = model->GetSegmentList();

	for (uint32_t i=0; i<numSegments; i++) {
		Segment& s = segments[i];
		if (s.parent != ~0u) {
			auto& m = matrices[i];
			m = identity4x4;
			m.m[3][0] = s.pivot.x;
			m.m[3][1] = s.pivot.y;
			m.m[3][2] = s.pivot.z;
			m = matrices[s.parent] * m;
		}
	}
}

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

	// transform the bind pose to the instance location
	NonAnimatedRebuild(
		transformComponent.m_matrix,
		instance->GetMeshComponent().GetModel());

	// these matrices are read by CalcSkeletonMatrices via InstanceDrawable::PrepareMatrixState
}

}
