#include "cdcCollide/CollisionManager.h" // for CollideMesh and CollideGeometries
#include "cdcCollide/SweptSphere.h" // for CollidePhantoms
#include "MultibodyAABBCollision.h"
#include "MultibodySystem.h"
#include "PhysicsBody.h"
#include "PhysicsGeometry.h"
#include "TimeStepIsland.h"

namespace cdc {

PhysicsBody *MultibodySystem::CreateBody() {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	auto *b = static_cast<PhysicsBodyImpl*>(mb->unusedBodies);
	if (!b) {
		// MULTIBODY_PrintWarning("WARNING: Out of space for Bodies!\n");
		return nullptr;
	}
	if (mb->lock & 1)
		return nullptr;
	mb->unusedBodies = b->next;
	b->Clear();
	b->prev = nullptr;
	b->next = mb->firstBody;
	if (b->next)
		b->next->prev = b;
	mb->nb++;
	return b;
}

void MultibodySystem::DestroyBody(PhysicsBody*) {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

PhysicsGeometry *MultibodySystem::CreateGeometry() {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

void MultibodySystem::DestroyGeometry(PhysicsGeometry*) {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

PhysicsJoint *MultibodySystem::CreateJoint() {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

void MultibodySystem::DestroyJoint(PhysicsJoint*) {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

void MultibodySystem::SetNearCallback(uint8_t, uint8_t, NearCallback*) {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

void MultibodySystem::SetContactCallback(uint8_t, uint8_t, ContactCallback*) {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

int32_t MultibodySystem::QueryBox(Vector3 const*, Vector3 const*, PhysicsGeometry**, int32_t) {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

int32_t MultibodySystem::QueryGeometry(Contact*, int32_t, PhysicsGeometry*) {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

void MultibodySystem::Relocate(Vector3 const*) {
	auto *mb = static_cast<MultibodySystemImpl*>(this);
	// TODO
}

// -------- //

void MultibodySystemImpl::TimeStepIslands(float timeStep, bool preserveForces) { // TimeStep.cpp:302
	if (false) {
		// TODO
		TimeStepIsland tsi;
		tsi.Setup();
		tsi.Process(nullptr);
		tsi.Update(preserveForces);

	} else {
		// HACK
		for (auto *body = firstBody; body; body = body->next)
			body->UpdatePosition(timeStep);
	}
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
	CollideGeometries(this); // -> cdcCollision/CollisionManager.h
}

void MultibodySystemImpl::StepImpl_CollidePhantoms() { // line 837
	CollidePhantoms(this); // -> cdcCollision/SweptSphere.h
}

void MultibodySystemImpl::StepImpl_FinishGeomCollision() { // line 851
	// TODO
}

void MultibodySystemImpl::StepImpl_CollideMesh(float dt) { // line 867
	CollideMesh(this, dt); // -> cdcCollision/CollisionManager.h
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
	StepImpl_TimeStep(timeStep, preserveForces); // vel += force; pos += vel;
	StepImpl_Finish();
}

}