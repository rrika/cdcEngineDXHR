#pragma once
#include <cstdint>

namespace cdc {

using VertIndex = uint32_t; // line 17
using FaceIndex = uint32_t; // line 18

struct MTriangle { // line 75
	Vector3 v0; // 0
	Vector3 v1; // 10
	Vector3 v2; // 20
};

struct BBox { // line 110
	Vector3 bMin;
	Vector3 bMax;
};

struct IndexedFace { // line 131
	VertIndex i0;
	VertIndex i1;
	VertIndex i2;
	uint8_t adjacencyFlags; // C
	uint8_t collisionFlags; // D
	uint16_t clientFlags; // E
	uint32_t materialType; // 10
};

}
