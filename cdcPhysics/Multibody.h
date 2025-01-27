#pragma once
#include <cstdint>

class Instance;
struct HModel;

namespace dtp { struct rigidbody; }

namespace cdc {

class Contact;
class Matrix;
class PhysicsBody;
class PhysicsGeometry;
class PhysicsJoint;
class Vector3;
struct MassProperties;
struct MeshInstance;

using NearCallback = bool(MeshInstance*, PhysicsGeometry*, PhysicsGeometry*); // line 60
using ContactCallback = bool(Contact*, int); // line 63

class MultibodySystem {
public:
	virtual ~MultibodySystem() = default;
	// virtual void PrintWarning(char*, char*) = 0;
	// virtual void PrintWarning(char*, ...) = 0;
	// virtual void UpdateWarning() = 0;
	// virtual void Report() = 0;
	// virtual void Init() = 0;
	// virtual void InstallCallbacks() = 0;
	// virtual void Step() = 0;
	// virtual float ComputeImpulse(Contact const*) = 0;
	// virtual float ComputeApproxImpulse(Contact const*) = 0;
	// virtual void InitBodyPosition(PhysicsBody*, MassProperties const*, Instance const*, int32_t) = 0;
	// virtual void InitBodyPosition(PhysicsBody*, MassProperties const*, Instance const*) = 0;
	// virtual void RemoveBodyFromRigidBodySystem(PhysicsBody*) = 0;
	// virtual void RemoveInstanceFromRigidBodySystem(Instance*) = 0;
	// virtual void RemoveHPrimFromRigidBodySystem(Instance*, HPrim*) = 0;
	// virtual void RemoveInstancesFromRigidBodySystem(Instance**, int32_t) = 0;
	// virtual void UpdateInstanceFromBody(Instance*, PhysicsBody const*) = 0;
	// virtual void method_44() = 0;
	// virtual void method_48() = 0;
	// virtual void method_4C() = 0;
	// virtual void UpdateInstanceMatricesFromBody(Instance*) = 0;
	// virtual void UpdateInstancesFromBodies() = 0;
	// virtual void method_54() = 0;
	// virtual void method_58() = 0;
	// virtual void method_5C() = 0;
	// virtual void method_60() = 0;
	// virtual void method_64() = 0;
	// virtual void method_68() = 0;
	// virtual void method_6C() = 0;
	// virtual void CreateAnimatedGeomsFromModel(Instance*, int32_t, dtp::rigidbody const*, bool) = 0;
	virtual bool CreateBodyFromModel(Instance*, int32_t, dtp::rigidbody const*, Matrix*, float) = 0;
	// virtual PhysicsBody* CreateBodyFromBone(Instance*, int32_t, int32_t, dtp::rigidbody const*, Matrix*) = 0;
	// virtual void CalcMassPropertiesFromHPrim(HPrim const*, MassProperties*) = 0;
	// virtual void CalcMassPropertiesFromHModel(HModel const*, int32_t, MassProperties*) = 0;
	virtual void CalcMassPropertiesFromHModel(HModel const*, MassProperties*) = 0;
	// virtual PhysicsGeometry* CreateGeometryFromHPrim(HPrim*, Vector3 const*, dtp::rigidbody const*, Instance*) = 0;
	// virtual void AddGeometriesToBody(PhysicsBody*, HModel const*, int32_t, Vector3 const*, dtp::rigidbody const*, Instance*) = 0;
	virtual void AddGeometriesToBody(PhysicsBody*, HModel const*, Vector3 const*, dtp::rigidbody const*, Instance*) = 0;
	// virtual void InitBodyProperties(PhysicsBody*, dtp::rigidbody const*) = 0;
	// virtual void SyncBodyProperties(PhysicsBody*, dtp::rigidbody const*) = 0;
	// virtual void ApplyExternalForces(uint32_t, Instance*, Instance*, Vector3 const*, Vector3 const*, Vector3 const*, Vector3 const*) = 0;
	// virtual void ExplosionToGeom(ExplosionData*, PhysicsGeometry*) = 0;
	// virtual void ExplosionToTarget(ExplosionData*, Instance*) = 0;
	// virtual void Explosion(ExplosionData*) = 0;

	PhysicsBody* CreateBody();
	void DestroyBody(PhysicsBody*);
	PhysicsGeometry* CreateGeometry();
	void DestroyGeometry(PhysicsGeometry*);
	PhysicsJoint* CreateJoint();
	void DestroyJoint(PhysicsJoint*);
	void SetNearCallback(uint8_t, uint8_t, NearCallback*);
	void SetContactCallback(uint8_t, uint8_t, ContactCallback*);
	int32_t QueryBox(Vector3 const*, Vector3 const*, PhysicsGeometry**, int32_t);
	int32_t QueryGeometry(Contact*, int32_t, PhysicsGeometry*);
	void Relocate(Vector3 const*);
};

class PhysicsBody { // line 299
public:
	void SetName(const char*);
	void SetMassProperties(MassProperties const&);
	void SetClientData(void*); // in practice this is an Instance* pointer
	// TODO
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
	// void SetCategory(uint8_t);
	// uint8_t GetCategory();
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

class PhysicsJoint { // line 543
public:
	// TODO
};

extern void MULTIBODY_PrintWarning(const char *fmt, ...);

}
