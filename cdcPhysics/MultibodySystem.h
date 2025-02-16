#pragma once
#include <cstdint>
#include "PhysicsJoint.h"
#include "Multibody.h"

namespace cdc {

struct MultibodyAABBCollision;
class PhysicsBodyImpl;
class PhysicsGeometryImpl;

struct MeshCollection {
	MeshInstance **meshInstances; // 0
	// uint32_t dword4;
	uint32_t numMeshInstances; // 8
};

struct TimeStepIslandParams { // line 63
	JointParams jtParams; // 0
	int16_t playerPhysicsWarnings; // 20
	float dt; // 24, see also jtParams.dt
	float sorOmega; // 28
	float beta; // 2C
};

class MultibodySystemImpl : public MultibodySystem {
public:
	friend class MultibodySystem;

	uint32_t numCollections = 8; // 18
	uint32_t numMeshInstancesPerCollection = 200; // 1C

	PhysicsBodyImpl *unusedBodies; // 2034
	PhysicsBodyImpl *firstBody; // 204C
	PhysicsGeometryImpl *firstGeometry; // 2050
	int32_t nb; // 2084
	int32_t nc; // 2094
	int32_t maxContactsUsed; // 2098
	MultibodyAABBCollision *m_aabbCollision; // 20D0
	MeshCollection *m_meshCollections; // 20D4
	int32_t lock; // 20DC
public:
	// ...

	MultibodySystemImpl();
	~MultibodySystemImpl();

	void TimeStepIslands(float timeStep, bool preserveForces);
	void StoreContactForces();

	void StepImpl_StartCollisionSetup(bool preserveForces);
	void StepImpl_FinishCollisionSetup();
	void StepImpl_StartGeomCollision();
	void StepImpl_CollidePhantoms();
	void StepImpl_FinishGeomCollision();
	void StepImpl_CollideMesh(float dt);
	void StepImpl_TimeStep(float timeStep, bool preserveForces);
	void StepImpl_Finish();

	void StepImpl_Atomic(float timeStep, bool preserveForces);
};

}
