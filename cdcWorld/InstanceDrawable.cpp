#include "InstanceDrawable.h"
#include "cdcWorld/cdcWorldTypes.h" // for dtp::Model
#include "cdcWorld/CalcSkeleton.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"
#include "cdcRender/CommonRenderDevice.h"
#include "cdcRender/Culling/BasicPrimitives.h"
#include "cdcRender/Culling/BasicPrimitives_inlines.h" // for SetFromMinMax
#include "cdcRender/PCDX11MatrixState.h"
#include "cdcRender/RenderMesh.h"

#include "cdcRender/PCDX11RenderDevice.h" // HACK
#include "cdcRender/PCDX11RenderModelInstance.h" // HACK
#include "game/DeferredRenderingObject.h" // HACK

namespace cdc {

InstanceDrawable *InstanceDrawable::s_pFirstDirty = nullptr;
InstanceDrawable *InstanceDrawable::s_pLastDirty = nullptr;

dtp::Model *MeshComponent::GetModel() {
	dtp::Model **models = m_instance->GetModels();
	return models[m_currentRenderModel];
}

dtp::Model *MeshComponent::GetBaseModel() {
	dtp::Model **models = m_instance->GetModels();
	return models[m_currentBaseModel];
}

void MeshComponent::SetModel(uint32_t index) {
	dtp::Model **models = m_instance->GetModels();
	dtp::Model *baseModel = models[m_currentBaseModel];
	SetModel(index, baseModel);
}

void MeshComponent::SetModel(uint32_t index, dtp::Model *baseModel) {
	dtp::Model **models = m_instance->GetModels();
	dtp::Model *model = models[index];
	auto& transform = m_instance->GetTransformComponent();

	// how many matrices are needed
	uint32_t numMatrices = model->GetNumSegments();
	if (transform.GetNotAnimated() == false)
		numMatrices += 1 + model->numVirtSegments;

	// allocate / reallocate as necessary
	if (transform.m_matrix) {
		if (model == baseModel) {
			m_currentRenderModel = index;

		} else {
			uint32_t numBaseMatrices = baseModel->GetNumSegments();
			if (transform.GetNotAnimated() == false)
				numBaseMatrices += 1 + baseModel->numVirtSegments;

			m_currentRenderModel = index;
			m_currentBaseModel = index;
			
			if (numMatrices != numBaseMatrices) {
				// TODO
			}
		}
	} else {
		transform.matrixBuffer = new cdc::Matrix[numMatrices];
		transform.m_matrix = transform.matrixBuffer;
		if (transform.GetNotAnimated() == false)
			transform.m_matrix++; // m_matrix[-1] is now a valid location

		// TODO

		m_currentRenderModel = index;
		m_currentBaseModel = index;
	}

	if (numMatrices <= 1 || (m_instance->object->dtpData->dword0 & 0x20000000))
		transform.m_rootMatrix = 0;
	else
		transform.m_rootMatrix = 1;

	// HACK
	m_instance->InitEditorPose(model);
}


InstanceDrawable::InstanceDrawable(Instance *instance) :
	m_instance(instance)
{
	uint32_t numModels = instance->GetNumModels();
	dtp::Model **models = instance->GetModels();

	m_renderModelInstances.resize(numModels);
	for (uint32_t i=0; i<numModels; i++)
		m_renderModelInstances[i] = g_renderDevice->createRenderModelInstance(models[i]->renderMesh);

	m_pMatrixState = g_renderDevice->createMatrixState();

	// this ensures that SceneLayer::Update/UpdateInstances creates a SceneEntity for this Instance
	AddToDirtyList();
}

void InstanceDrawable::EnableNoDraw() {
	bool before = QueryNoDraw();
	flags |= 4;
	bool after = QueryNoDraw();
	if (before != after)
		AddToDirtyList();
}

void InstanceDrawable::DisableNoDraw() {
	bool before = QueryNoDraw();
	flags &= ~4;
	bool after = QueryNoDraw();
	if (before != after)
		AddToDirtyList();
}

bool InstanceDrawable::QueryNoDraw() const {
	return (flags & 0xF4) != 0;
}

RenderModelInstance *InstanceDrawable::getSelectedRMI() {
	MeshComponent& meshComponent = m_instance->GetMeshComponent();
	return m_renderModelInstances[meshComponent.GetCurrentRenderModelIndex()];
}

void InstanceDrawable::GetBoundingVolume(BasicCullingVolume *volume) {

	Vector center, min, max;
	float radius;

	if (GetBoundingSphere(&center, &radius)) {
		CullingSphere sphere {center};
		const float minimumRadius = 1.0f / 0x4000;
		if (radius < minimumRadius)
			radius = minimumRadius;
		sphere.m_sphereEq.w = radius;
		volume->m_data.sphere = sphere;
		volume->m_type = kVolumeSphere;

	} else if (GetBoundingBox(&min, &max)) {
		CullingBox box;
		box.SetFromMinMax({min}, {max});
		volume->m_data.box = box;
		volume->m_type = kVolumeBox;

	} else {
		volume->m_type = kVolumeEverything;
	}
}

// called from cdc::Scene::RenderWithoutCellTracing for example
void InstanceDrawable::draw(Matrix *matrix, float) { // line 1243

	// When the render model uses bones, the matrices will be taken from
	// the transform component, else the matrix argument will be used.
	// It's provided from SceneEntity::m_matrix, which in turn is populated
	// from the first matrix of the transform component via UpdateInstances.

	if (m_renderModelInstances.empty())
		return; // HACK

	// TODO
	MeshComponent& meshComponent = m_instance->GetMeshComponent();
	dtp::Model *model = meshComponent.GetModel();
	cdc::RenderModelInstance *rmi = m_renderModelInstances[meshComponent.GetCurrentRenderModelIndex()];

	if (model && rmi) {
		if (dynamic_cast<DeferredRenderingObject::Drawable*>(this)) { // HACK
			auto *scene = static_cast<PCDX11RenderModelInstance*>(rmi)->renderDevice->getScene();
			Matrix project = scene->projectMatrix;
			project.m[0][2] *= 0.00001;
			project.m[1][2] *= 0.00001;
			project.m[2][2] *= 0.00001;
			project.m[3][2] *= 0.00001;
			rmi->SetProjectionOverride(&project);
		}

		// if (skydome) {
		//	...
		// } else {
		PrepareMatrixState(matrix, model, rmi, false); // matrix is only used for 0 bones case
		// }
		rmi->recordDrawables(m_pMatrixState);

		for (auto& addRMI : m_additionalModelInstances) {
			addRMI.m_pRMI->recordDrawables(m_pMatrixState);
		}
	}
}

bool InstanceDrawable::GetBoundingSphere(Vector *pCenter, float *pRadius) {
	// TODO
	return false;
}

bool InstanceDrawable::GetBoundingBox(Vector *pMin, Vector *pMax) {
	// HACK
	MeshComponent& meshComponent = m_instance->GetMeshComponent();
	cdc::RenderModelInstance *rmi = m_renderModelInstances.at(meshComponent.GetCurrentRenderModelIndex());
	cdc::RenderMesh const *rm = rmi->GetRenderMesh();
	return rm->getBoundingBox(*(Vector3*)pMin, *(Vector3*)pMax);
}

InstanceDrawable::~InstanceDrawable() {
	for (auto *rmi : m_renderModelInstances)
		delete rmi;
	for (auto& addRMI : m_additionalModelInstances)
		if (addRMI.m_hasOwnership)
			delete addRMI.m_pRMI;
	RemoveFromDirtyList();
}

void InstanceDrawable::PrepareMatrixState(Matrix *matrix, dtp::Model *model, RenderModelInstance *rmi, bool force) { // line 1880
	if (true || force) {
		auto boneCount = rmi->GetRenderMesh()->getBoneCount();
		m_pMatrixState->resize(boneCount);
		if (boneCount) {
			auto *inMatrices = m_instance->GetTransformComponent().m_matrix;
			auto *poseData = static_cast<cdc::PCDX11MatrixState*>(m_pMatrixState)->poseData;
			auto *outMatrices = reinterpret_cast<cdc::Matrix*>(poseData->getMatrix(0));
			outMatrices[0] = inMatrices[0];
			CalcSkeletonMatrices(model, inMatrices, boneCount, m_pMatrixState);
		} else {
			auto *poseData = static_cast<cdc::PCDX11MatrixState*>(m_pMatrixState)->poseData;
			auto *pMatrix = reinterpret_cast<cdc::Matrix*>(poseData->getMatrix(0));
			*pMatrix = *matrix;
		}
	}
}

void InstanceDrawable::AddToDirtyList() { // 2038
	// dirty list is processed by SceneLayer::Update/UpdateInstances
	if (flags & 8)
		return;
	flags |= 8;

	m_pPrevDirty = s_pLastDirty;
	m_pNextDirty = nullptr;
	if (s_pLastDirty) {
		s_pLastDirty->m_pNextDirty = this;
		s_pLastDirty = this;
		// TODO
	} else {
		s_pFirstDirty = this;
		s_pLastDirty = this;
	}
}

void InstanceDrawable::RemoveFromDirtyList() { // 2052
	if (!(flags & 8))
		return;
	flags &= ~8;

	if (m_pPrevDirty)
		m_pPrevDirty->m_pNextDirty = m_pNextDirty;
	else
		s_pFirstDirty = m_pNextDirty;

	if (m_pNextDirty)
		m_pNextDirty->m_pPrevDirty = m_pPrevDirty;
	else
		s_pLastDirty = m_pPrevDirty;

	// HACK
	m_pNextDirty = nullptr;
	m_pPrevDirty = nullptr;
}

}
