#include "InstncG2.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcMath/Math.h"
#include "cdcWorld/cdcWorldTypes.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/InstanceManager.h"

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

void G2Instance_BuildAllTransforms() { // guessed name
	// HACK
	G2Instance_BuildTransformsForList(InstanceManager::s_chain);
}

void G2Instance_BuildTransformsForList(Instance *instance) { // line 176
	for (; instance; instance = instance->next) {
		// if (instance == ...) break;
		G2Instance_BuildTransforms(instance);
	}
}

bool G2Instance_NeedsBuildTransforms(Instance *instance) { // line 218
	// TODO
	return true;
}

static void G2Instance_CollectBoneUsage(Instance *instance) {

}

void G2Instance_BuildTransforms(Instance *instance) { // line 326
	if (true) { // TODO
		if (G2Instance_NeedsBuildTransforms(instance)) {
			// this is in a separate function in DXHR
			// instance->GetTransformComponent().SwapMatrixPools();
			// instance->GetTransformComponent().InvalidateDrawableMatrix();
			G2Instance_RebuildTransforms(instance);
			G2Instance_CollectBoneUsage(instance);
		} else if (false) {
			G2Instance_CollectBoneUsage(instance);
		}
		// for (Instance *child = instance->linkChild; child; child = child->nextSibling)
		// 	if (child.GetHierarchyComponent().SomeFunction())
		// 		G2Instance_BuildTransforms(child);
	}
}

void G2Instance_RebuildTransforms(Instance *instance) { // line 386/404
	// matrices are allocated in MeshComponent::SetModel
	// instance->GetTransformComponent().InvalidateDrawableMatrix();
	Matrix *matrices = instance->GetTransformComponent().m_matrix;
	if (matrices) {

		if (instance->enableOverridePose) {
			instance->BuildEditorTransforms(matrices);

		} else if (AnimComponentV2 *ac2 = instance->animComponentV2) {
			ac2->matrices = matrices;
			ac2->BuildTransforms();
		}
		G2Instance_PostRebuildTransforms(instance);
	}
}


void G2Instance_PostRebuildTransforms(Instance *instance) { // line 534

}

// HACK
void G2Instance_SetTransformsToIdentity(Instance *instance) { // line 667

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
