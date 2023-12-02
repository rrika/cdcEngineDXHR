#include "CalcSkeleton.h"
#include "cdcMath/MatrixInlines.h"
#include "rendering/PCDX11MatrixState.h"
#include "cdcWorld/cdcWorldTypes.h"

namespace cdc {

// called from InstanceDrawable::PrepareMatrixState
void CalcSkeletonMatrices(dtp::Model *model, Matrix *src, uint32_t boneCount, IMatrixState *dst) {
	auto *poseData = static_cast<cdc::PCDX11MatrixState*>(dst)->poseData;
	auto *pMatrix = reinterpret_cast<cdc::Matrix*>(poseData->getMatrix(0));

	Segment *segments = model->GetSegmentList();
	uint32_t numSegments = model->GetNumSegments();
	uint32_t lesserCount = numSegments < boneCount-1 ? numSegments : boneCount-1;

	static Vector rest_pose[256];
	rest_pose[0] = segments->pivot;
	for (uint32_t i=1; i<lesserCount; i++)
		rest_pose[i] = rest_pose[segments[i].parent] + segments[i].pivot;

	static Vector3 pose[256];
	for (uint32_t i=1; i<lesserCount; i++)
		pose[i] = src[i] * Vector3{-rest_pose[i]};

	for (uint32_t i=0; i<numSegments; i++) {
		Matrix m = src[i];
		m.m[3][0] = pose[i].x;
		m.m[3][1] = pose[i].y;
		m.m[3][2] = pose[i].z;
		m.m[3][3] = 1.0f;
		pMatrix[i] = m;
	}
	for (uint32_t i=numSegments; i<boneCount; i++) {
		pMatrix[i] = identity4x4;
	}
}

}
