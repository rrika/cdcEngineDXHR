#pragma once
#include "Multibody.h"

namespace cdc {

class PhysicsGeometryImpl : public PhysicsGeometry { // line 26
	// Vector3 dx;
	// Quat dq;
	// float restitution;

	// __m128 oword0;
	// __m128 oword10;
	// __m128 oword20;
	// __m128 quatMaybe30;
	// __m128 quatMaybe40;
	// uint8_t gap50[16];
	// float float60;
	// float float64;
	// uint32_t dword68;
	// float float6C;
	// float float70;

	float mu; // 74
	PhysicsGeometryImpl* prev; // 78
	PhysicsGeometryImpl* next; // 7C
	PhysicsBodyImpl* body; // 80
	// PhysicsGeometryImpl* bnext;
	// void *clientData;
	// cdc::ImplicitGeometryDestructionCallback *clientDestruct;
	// uint32_t uniqueIndex;
	// unsigned char category;
	// CollisionPrimitiveImpl collisionPrim;

	// uint32_t body80;
	// uint32_t dword84;
	// uint32_t dword88;
	// uint32_t dword8C;
	// MultibodyAABBCollision *mbAABB; // 90
	// uint32_t dword94;
	// uint8_t gap98[4];
	// uint8_t byte9C;
	// uint8_t byte9D;
	// uint8_t byte9E;
	// uint8_t byte9F;
};

}
