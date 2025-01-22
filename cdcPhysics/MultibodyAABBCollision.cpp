#include "cdcCollide/AABBCollision.h"
#include "cdcPhysics/MultibodyAABBCollision.h"

namespace cdc {

MultibodyAABBCollision *Create() { // line 170
	auto *aabbCollision = new MultibodyAABBCollision;
	aabbCollision->Allocate();
	return aabbCollision;
}

void MultibodyAABBCollision::Allocate() { // line 188
	// TODO
	aabbCollision::Allocate();
}

uint32_t MultibodyAABBCollision::PrimQuery( // line 1721
	AABBCollisionDataNode **outPtrs,
	uint32_t maxOutPtrs,
	AABBCollisionNode const& box)
{
	if (primTreeNumNodes) {
		uint32_t numOutPtrs = primTreeRootNode->CollideTree(outPtrs, maxOutPtrs, box, /*fn=*/nullptr);
		if (numOutPtrs > maxOutPtrs)
			numOutPtrs = maxOutPtrs;
	}
	// TODO
	return numOutPtrs;
}



void MultibodyAABBCollision::MeshTreeResetDataNodes() { // line 1999

}

uint8_t MultibodyAABBCollision::ColSetup_InitMeshInstances(AABBCollisionJobInstance*& aabbMeshJobInsts) { // line 2061
	if (byte0 == 0)
		return;

	// TODO
	MeshTreeResetDataNodes();
	// TODO
	if (meshDataNumNodes > 32) {
		// TODO
		aabbMeshJobInsts = new AABBCollisionJobInstance[4];
		aabbMeshJobInsts[0].Init();
		aabbMeshJobInsts[1].Init();
		aabbMeshJobInsts[2].Init();
		aabbMeshJobInsts[3].Init();
		// TODO
		return 4;

	} else if (meshDataNumNodes > 4) {
		// TODO
		aabbMeshJobInsts = new AABBCollisionJobInstance[2];
		aabbMeshJobInsts[0].Init();
		aabbMeshJobInsts[1].Init();
		// TODO
		return 2;

	} else {
		// TODO
		return 0;
	}
}

uint8_t MultibodyAABBCollision::ColSetup_InitMeshInstances2(AABBCollisionJobInstance*& aabbMeshJobInsts) {
	// TODO
}

void MultibodyAABBCollision::CollisionSetup_Start() { // line 2293
	AABBCollisionJobInstance *jobInsts;
	AABBCollisionJobInstance *jobInsts2;
	uint8_t count = ColSetup_InitMeshInstances(jobInsts);
	ColSetup_InitMeshInstances2(jobInsts2);
	ColSetup_RunJobs(jobInsts, count, jobInsts2)

}

}
