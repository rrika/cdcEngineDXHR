#include "RMIDrawableBase.h"
#include "../rendering/CommonRenderDevice.h"
#include "../rendering/RenderModelInstance.h"
#include "../rendering/PCDX11MatrixState.h"

RMIDrawableBase::RMIDrawableBase(cdc::RenderMesh *model) {
	rmi = cdc::g_renderDevice->createRenderModelInstance(model);
	matrixState = cdc::g_renderDevice->createMatrixState();
}

void RMIDrawableBase::draw(cdc::Matrix *matrix, float) {
	if (true) { // matrixState->isValid()) {
		matrixState->resize(0);
		auto *pMatrix = reinterpret_cast<cdc::Matrix*>(
			static_cast<cdc::PCDX11MatrixState*>(matrixState)->poseData->getMatrix(0));
		*pMatrix = *matrix;
	}
	rmi->recordDrawables(matrixState);
}
