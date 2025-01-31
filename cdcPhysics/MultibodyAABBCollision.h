#pragma once
#include <cstdint>

namespace cdc {

class MultibodySystemImpl;
struct AABBCollisionDataNode;
struct AABBCollisionTreeNode;
struct AABBCollisionJobInstance;
struct AABBCollisionNode;
struct MeshInstance;

class MultibodyAABBCollision {
	uint8_t byte0;
	AABBCollisionDataNode *meshDataFirstNode = nullptr; // 10
	uint32_t meshDataNumNodes = 0; // 14
	uint32_t meshDataNumNewNodes = 0; // 18
	bool primTreeDirty = false; // 1C
	AABBCollisionTreeNode *primTreeRootNode = nullptr; // 20
	uint32_t primTreeNumNodes = 0; // 28
	MultibodySystemImpl *multibodySystem = nullptr; // 5C
	uint32_t maxMeshInstances; // 64
public:
	static MultibodyAABBCollision *Create(MultibodySystemImpl *mb);
	MultibodyAABBCollision() {} // line 224
	void Allocate(MultibodySystemImpl *mb);
	void MeshInstAdd(MeshInstance *mi);
	uint32_t PrimQuery(AABBCollisionDataNode **outPtrs, uint32_t maxOutPtrs, AABBCollisionNode const& box);
	void MeshTreeResetDataNodes();
	uint8_t ColSetup_InitMeshInstances(AABBCollisionJobInstance*& aabbMeshJobInsts);
	uint8_t ColSetup_InitMeshInstances2(AABBCollisionJobInstance*& aabbMeshJobInsts);
	void CollisionSetup_Start();
};

}
