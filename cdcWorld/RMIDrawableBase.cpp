#include "RMIDrawableBase.h"
#include "rendering/Culling/BasicPrimitives.h"
#include "rendering/Culling/BasicPrimitives_inlines.h"
#include "rendering/CommonRenderDevice.h"
#include "rendering/RenderMesh.h"
#include "rendering/RenderModelInstance.h"
#include "rendering/PCDX11MatrixState.h"

RMIDrawableBase::RMIDrawableBase(cdc::RenderMesh *model) {
	rmi = cdc::g_renderDevice->createRenderModelInstance(model);
	matrixState = cdc::g_renderDevice->createMatrixState();
}

RMIDrawableBase::~RMIDrawableBase() {
	delete rmi;
	delete matrixState;
}

void RMIDrawableBase::GetBoundingVolume(cdc::BasicCullingVolume *volume) {

	cdc::Vector3 min, max;
	cdc::RenderMesh const *rm = rmi->GetRenderMesh();

	if (rm->getBoundingBox(min, max)) {
		cdc::CullingBox box;
		box.SetFromMinMax(min, max);
		volume->m_data.box = box;
		volume->m_type = cdc::kVolumeBox;
	}
}

void RMIDrawableBase::draw(cdc::Matrix *matrix, float) {
	if (true) { // matrixState->isValid()) {
		matrixState->resize(0);
		auto *poseData = static_cast<cdc::PCDX11MatrixState*>(matrixState)->poseData;
		auto *pMatrix = reinterpret_cast<cdc::Matrix*>(poseData->getMatrix(0));
		float *pVector = poseData->getVector(0);
		*pMatrix = *matrix;
		pVector[0] = pMatrix->m[0][3];
		pVector[1] = pMatrix->m[1][3];
		pVector[2] = pMatrix->m[2][3];
		pVector[3] = 1.0f;
	}
	rmi->recordDrawables(matrixState);
}
