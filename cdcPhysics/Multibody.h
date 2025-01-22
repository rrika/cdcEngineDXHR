#pragma once

namespace cdc {

class MultibodySystem {
public:
	// empty
};

class PhysicsGeometry { // line 437
public:
	// void SetPhysicsBox(const cdc::Vector3*, const cdc::Vector3*, const cdc::Quat*);
	// void SetPhysicsSphere(float, const cdc::Vector3*);
	// void SetPhysicsCapsule(float, float, const cdc::Vector3*, const cdc::Quat*);
	// void SetPhysicsPlane(const cdc::Vector3*, float);
	// void SetPhysicsCDCGeom(cdc::Geom*, const cdc::Vector3*, const cdc::Quat*);
	// const cdc::PhysicsBody* GetBody();
	// cdc::PhysicsBody* GetBody();
	// void SetClientData(void *);
	// void * GetClientData();
	// void SetCategory(unsigned char);
	// unsigned char GetCategory();
	// void SetDestructionCallback(Type0x126f8 /* TODO: figure out how to name it */*);
	// void SetFriction(float);
	// float GetFriction();
	// void SetRestitution(float);
	// float GetRestitution();
	// int32_t GetContacts(cdc::ContactReport*, int32_t);
	// void AddBuoyancyForce(const cdc::Vector3*, float, float, const cdc::Vector3*, float, float);
	// void GetClosestPoint(cdc::Vector3*, cdc::Vector3*, const cdc::Vector3*);
	// cdc::Vector3 GetProbeOrClosestPoint(const cdc::Vector3*, const cdc::Vector3*, cdc::Matrix*, cdc::Vector3*, cdc::Quat*);
	// bool ProbeGeom(const cdc::Vector3*, const cdc::Vector3*, float*, cdc::Vector3*, bool, uint32_t);
	// float GetGeomExtentInDir(const cdc::Vector3*);
	// void SetRelativePosition(const cdc::Vector3*);
	// cdc::Vector3 GetRelativePosition();
	// void SetRelativeQuaternion(const cdc::Quat*);
	// cdc::Quat GetRelativeQuaternion();
	// void SetRelativePositionAndQuaternion(const cdc::Vector3*, const cdc::Quat*);
	// const cdc::CollisionPrimitive* GetCollisionPrim();
	// cdc::CollisionPrimitive* GetCollisionPrim();
	// void PhysicsGeometry(const cdc::PhysicsGeometry*);
	// void PhysicsGeometry();
	// cdc::PhysicsGeometry* operator=(const cdc::PhysicsGeometry*);
};

}
