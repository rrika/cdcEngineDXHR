#pragma once
#include <cstdint>
#include "Multibody.h"

namespace cdc {

struct MultibodyAABBCollision;
class PhysicsBodyImpl;
class PhysicsGeometryImpl;

class MultibodySystemImpl : public MultibodySystem {
	friend class MultibodySystem;

	PhysicsBodyImpl *unusedBodies; // 2034
	PhysicsBodyImpl *firstBody; // 204C
	PhysicsGeometryImpl *firstGeometry; // 2050
	int32_t nb; // 2084
	int32_t nc; // 2094
	int32_t maxContactsUsed; // 2098
	MultibodyAABBCollision *m_aabbCollision;
	int32_t lock; // 20DC
public:
	// ...

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
