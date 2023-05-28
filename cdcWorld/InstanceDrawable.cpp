#include "InstanceDrawable.h"
#include "cdcObjects/Object.h"
#include "cdcWorld/cdcWorldTypes.h" // for dtp::Model
#include "cdcWorld/Instance.h"
#include "rendering/CommonRenderDevice.h"
#include "rendering/Culling/BasicPrimitives.h"
#include "rendering/PCDX11MatrixState.h"

namespace cdc {

InstanceDrawable *InstanceDrawable::s_pFirstDirty = nullptr;
InstanceDrawable *InstanceDrawable::s_pLastDirty = nullptr;

dtp::Model *MeshComponent::GetModel() {
	dtp::Model **models = m_instance->GetModels();
	return models[m_currentRenderModel];
}

void MeshComponent::SetModel(uint32_t index) {
	dtp::Model **models = m_instance->GetModels();
	dtp::Model *baseModel = models[m_currentBaseModel];
	SetModel(index, baseModel);
}

void MeshComponent::SetModel(uint32_t index, dtp::Model *baseModel) {
	dtp::Model **models = m_instance->GetModels();
	dtp::Model *model = models[m_currentRenderModel];
	auto& transform = m_instance->GetTransformComponent();

	// how many matrices are needed
	uint32_t numMatrices = model->GetNumSegments();
	if ((transform.m_flags & 4 /*m_flagNotAnimated*/) == 0)
		numMatrices += model->numVirtSegments;

	// allocate / reallocate as necessary
	if (transform.m_matrix) {
		if (model == baseModel) {
			m_currentRenderModel = index;

		} else {
			uint32_t numBaseMatrices = baseModel->GetNumSegments();
			if ((transform.m_flags & 4 /*m_flagNotAnimated*/) == 0)
				numBaseMatrices += baseModel->numVirtSegments;

			m_currentRenderModel = index;
			m_currentBaseModel = index;
			
			if (numMatrices != numBaseMatrices) {
				// TODO
			}
		}
	} else {
		transform.m_matrix = new cdc::Matrix[numMatrices];

		// TODO

		m_currentRenderModel = index;
		m_currentBaseModel = index;
	}

	if (numMatrices <= 1 || (m_instance->object->dtpData->dword0 & 0x20000000))
		transform.m_rootMatrix = 0;
	else
		transform.m_rootMatrix = 1;
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

void InstanceDrawable::GetBoundingVolume(BasicCullingVolume *volume) {
	// HACK
	BasicCullingVolume everything;
	*volume = everything;
}

void InstanceDrawable::draw(Matrix *, float) {

	if (m_renderModelInstances.empty())
		return; // HACK

	// TODO
	MeshComponent& meshComponent = m_instance->GetMeshComponent();
	dtp::Model *model = meshComponent.GetModel();
	cdc::RenderModelInstance *rmi = m_renderModelInstances[meshComponent.GetCurrentRenderModelIndex()];

	if (model && rmi) {
		// HACK
		cdc::Matrix *matrix = m_instance->GetTransformComponent().m_matrix;

		m_pMatrixState->resize(0);
		auto *poseData = static_cast<cdc::PCDX11MatrixState*>(m_pMatrixState)->poseData;
		auto *pMatrix = reinterpret_cast<cdc::Matrix*>(poseData->getMatrix(0));
		float *pVector = poseData->getVector(0);
		*pMatrix = *matrix;
		pVector[0] = pMatrix->m[0][3];
		pVector[1] = pMatrix->m[1][3];
		pVector[2] = pMatrix->m[2][3];
		pVector[3] = 1.0f;

		rmi->recordDrawables(m_pMatrixState);
	}
}

void InstanceDrawable::AddToDirtyList() { // 2038
	// dirty list is processed by SceneLayer::Update/UpdateInstances
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
	if (m_pPrevDirty)
		m_pPrevDirty->m_pNextDirty = m_pNextDirty;
	else
		s_pFirstDirty = m_pNextDirty;

	if (m_pNextDirty)
		m_pNextDirty->m_pPrevDirty = m_pPrevDirty;
	else
		s_pLastDirty = m_pPrevDirty;
}

}
