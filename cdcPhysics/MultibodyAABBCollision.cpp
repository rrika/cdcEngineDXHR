#include "cdcCollide/AABBCollision.h"
#include "cdcCollide/Mesh.h"
#include "cdcPhysics/MultibodyAABBCollision.h"

namespace cdc {

MultibodyAABBCollision *Create(MultibodySystemImpl *mb) { // line 170
	auto *aabbCollision = new MultibodyAABBCollision;
	aabbCollision->Allocate(mb);
	return aabbCollision;
}

void MultibodyAABBCollision::Allocate(MultibodySystemImpl *mb) { // line 188
	multibodySystem = mb;
	maxMeshInstances = 2000; // Note: This is more than 8 * 200 from
		// MultibodySystemImpl::numCollections = 8
		// MultibodySystemImpl::numMeshInstancesPerCollection = 200
	meshDataFirstNode = new AABBCollisionDataNode[maxMeshInstances];
	primTreeRootNode = new AABBCollisionTreeNode[/*TODO*/ 2*maxMeshInstances];
	AABBCollision::Allocate();
}

void MultibodyAABBCollision::MeshInstAdd(MeshInstance *mi) { // line 338
	uint32_t totalMeshNodes = meshDataNumNodes + meshDataNumNewNodes;
	for (uint32_t i = 0; i < totalMeshNodes; i++)
		if (meshDataFirstNode[i].m_client == mi)
			return;

	AABBCollisionNode node;
	node.SetBBox(
		{mi->m_bbox.bMin + mi->m_streamOffset},
		{mi->m_bbox.bMax + mi->m_streamOffset}
	);
	if (totalMeshNodes < maxMeshInstances) {
		meshDataFirstNode[totalMeshNodes].SetDataNode(node, (void*)mi, false, 0);
		meshDataNumNewNodes++;
	}
}

uint32_t MultibodyAABBCollision::PrimQuery( // line 1721
	AABBCollisionDataNode **outPtrs,
	uint32_t maxOutPtrs,
	AABBCollisionNode const& box)
{
	uint32_t numOutPtrs = 0;
	if (primTreeNumNodes) {
		numOutPtrs = primTreeRootNode->CollideTree(outPtrs, maxOutPtrs, box, /*fn=*/nullptr);
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
		return 0;

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
	return 0;
}

void MultibodyAABBCollision::CollisionSetup_Start() { // line 2293
	AABBCollisionJobInstance *jobInsts;
	AABBCollisionJobInstance *jobInsts2;
	uint8_t count = ColSetup_InitMeshInstances(jobInsts);
	ColSetup_InitMeshInstances2(jobInsts2);
	// ColSetup_RunJobs(jobInsts, count, jobInsts2);

}

}
