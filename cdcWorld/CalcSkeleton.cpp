#include <cstring>
#include "CalcSkeleton.h"
#include "cdcMath/MatrixInlines.h"
#include "rendering/PCDX11MatrixState.h"
#include "cdcWorld/cdcWorldTypes.h"

namespace cdc {

// called from InstanceDrawable::PrepareMatrixState
void CalcSkeletonMatrices(dtp::Model *model, Matrix *src, uint32_t matrixCount, IMatrixState *dst) {

	// segments:  root bone at index 0
	// src:       root bone at index 1
	// rest_pose: root bone at index 1
	// pose:      root bone at index 1
	// matrixCount counts from one before root
	// numSegments counts from root
	// lesserCount counts from root

	auto *poseData = static_cast<cdc::PCDX11MatrixState*>(dst)->poseData;
	auto *pMatrix = reinterpret_cast<cdc::Matrix*>(poseData->getMatrix(0));

	Segment *segments = model->GetSegmentList();
	uint32_t numSegments = model->GetNumSegments();
	uint32_t lesserCount = numSegments < (matrixCount-1) ? numSegments : (matrixCount-1);

	// accumulate the rest pose from the segment pivots
	static Vector rest_pose[256];
	memset(rest_pose, 0, sizeof(rest_pose));
	rest_pose[0] = segments->pivot;
	for (uint32_t i=1; i<lesserCount+1; i++)
		rest_pose[i] = rest_pose[segments[i].parent] + segments[i].pivot;

	static Vector3 pose[256];
	memset(pose, 0, sizeof(pose));
	for (uint32_t i=0; i<lesserCount; i++)
		pose[i+1] = src[i+1] * Vector3{-rest_pose[i+1]};

	for (uint32_t i=1; i<numSegments; i++) {
		Matrix m = src[i];
		m.m[3][0] = pose[i].x;
		m.m[3][1] = pose[i].y;
		m.m[3][2] = pose[i].z;
		m.m[3][3] = 1.0f;
		pMatrix[i] = m;
	}
	for (uint32_t i=numSegments; i<matrixCount; i++) {
		pMatrix[i] = identity4x4;
	}
}

}
