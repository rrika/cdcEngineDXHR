#pragma once
#include <cstdint>

namespace cdc {

struct AABBCollisionDataNode;
struct AABBCollisionNode;

class MultibodyAABBCollision {
	uint8_t byte0;
	AABBCollisionDataNode *meshDataFirstNode; // 10
	uint32_t meshDataNumNodes; // 14
	uint32_t meshDataNumNewNodes; // 18
	bool primTreeDirty; // 1C
	AABBCollisionTreeNode *primTreeRootNode; // 20
	uint32_t primTreeNumNodes; // 28
public:
	static MultibodyAABBCollision *Create();
	MultibodyAABBCollision() {} // line 224
	void Allocate();
	uint32_t PrimQuery(AABBCollisionDataNode **outPtrs, uint32_t maxOutPtrs, AABBCollisionNode const& box);
	void MeshTreeResetDataNodes();
	uint8_t ColSetup_InitMeshInstances(AABBCollisionJobInstance*& aabbMeshJobInsts);
	uint8_t ColSetup_InitMeshInstances2(AABBCollisionJobInstance*& aabbMeshJobInsts);
	void CollisionSetup_Start();
};

}
