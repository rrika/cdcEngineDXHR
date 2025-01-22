#include "MultibodySystem.h"
#include "cdcCollide/CollisionManager.h" // for CollideMesh and CollideGeometries
#include "cdcCollide/SweptSphere.h" // for CollidePhantoms

namespace cdc {

void MultibodySystemImpl::TimeStepIslands(float timeStep, bool preserveForces) { // TimeStep.cpp:302
	// TODO
}

void MultibodySystemImpl::StoreContactForces() {
	// TODO
}

void MultibodySystemImpl::StepImpl_StartCollisionSetup(bool preserveForces) { // line 787
	m_aabbCollision->CollisionSetup_Start();
}

void MultibodySystemImpl::StepImpl_FinishCollisionSetup() { // line 810
	// TODO
	// AABBCollision::WaitUntilDone
  	// MultibodyAABBCollision::ColSetup_UnInitMeshInstances
}

void MultibodySystemImpl::StepImpl_StartGeomCollision() { // line 820
	CollideGeometries(this);
}

void MultibodySystemImpl::StepImpl_CollidePhantoms() { // line 837
	CollidePhantoms(this);
}

void MultibodySystemImpl::StepImpl_FinishGeomCollision() { // line 851
	// TODO
}

void MultibodySystemImpl::StepImpl_CollideMesh(float dt) { // line 867
	CollideMesh(this, dt);
}

void MultibodySystemImpl::StepImpl_TimeStep(float timeStep, bool preserveForces) { // line 882
    lock |= 8;
    if (maxContactsUsed >= nc)
        maxContactsUsed = nc;

	if (timeStep > 0.0f)
		TimeStepIslands(timeStep, preserveForces);
}

void MultibodySystemImpl::StepImpl_Finish() { // line 901
	StoreContactForces();
	lock = 0;
}

void MultibodySystemImpl::StepImpl_Atomic(float timeStep, bool preserveForces) { // line 918
	StepImpl_StartCollisionSetup(preserveForces);
	StepImpl_FinishCollisionSetup();
	StepImpl_StartGeomCollision();
	StepImpl_CollidePhantoms();
	StepImpl_FinishGeomCollision();
	StepImpl_CollideMesh(timeStep);
	StepImpl_TimeStep(timeStep, preserveForces);
	StepImpl_Finish();
}

}