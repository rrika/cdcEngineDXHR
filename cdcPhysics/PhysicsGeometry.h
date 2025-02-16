#pragma once
#include "Multibody.h"

namespace cdc {

class Geom;
struct AABBPrimBox;

class PhysicsGeometryImpl : public PhysicsGeometry { // line 26
public:
	enum Type : uint8_t {
		Box = 0,
		Capsule = 1,
		Plane = 2,
		Sphere = 3,
		CDCGeom = 4,
		Unknown = 5
	};

	Vector3 dx; // 0
	Vector3 x; // 10
	Vector3 x0; // 20
	Quat dq; // 30
	Quat q; // 40
	Vector3 width; // 50
	float radius; // 60
	float length; // 64
	Geom *geom; // 68
	float volume; // 6C
	float restitution; // 70
	float mu; // 74, friction
	PhysicsGeometryImpl *prev; // 78
	PhysicsGeometryImpl *next; // 7C
	PhysicsBodyImpl *body; // 80
	PhysicsGeometryImpl *bnext; // 84
	void *clientData; // 88
	// cdc::ImplicitGeometryDestructionCallback *clientDestruct;
	// uint32_t uniqueIndex;
	MultibodyAABBCollision *mbAABB; // 90
	AABBPrimBox *primAABB; // 94
	// uint32_t dword98;
	Type type; // 9C
	uint8_t category; // 9D
	uint8_t flags; // 9E
	bool tempPrimitive; // 9F
};

}
