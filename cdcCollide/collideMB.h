#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

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

enum CollideCode { // line 194
	NO_HIT = 0,
	HIT = 1,
	HIT_INSIDE = 2,
	HIT_CYLINDER_ENDCAP_X = 3,
	HIT_CYLINDER_ENDCAP_Y = 4
};

CollideCode CollideSegmentAndAlignedBox(
	float& lambda,
	Vector3Arg s,
	Vector3Arg d,
	BBox& bbox);

CollideCode CollideSegmentAndTri( // line 287
	float *lambda,
	Vector3 *normal,
	Vector3Arg s,
	Vector3Arg d,
	MTriangle const& tri);

}
