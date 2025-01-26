#pragma once
#include <cstdint>
#include "cdcCollide/collideMB.h"
#include "cdcMath/Math.h"

namespace cdc {

struct IndexedFace;

struct AABBNode { // line 52
	float f_min[3]; // 0
	float f_max[3]; // C
	uint32_t m_numFaces : 8; // 18
	uint32_t m_index : 24; // 1C
};

struct SegmentResult { // line 86
	IndexedFace *face;
	float lambda;
};

struct MTriangle;

struct Mesh { // line 104

	enum VertexType : uint16_t { // line 107
		VERTEX_INT16 = 0,
		VERTEX_FLOAT32 = 1
	};

	float float0[8]; // m_box
	void *vertices; // 20
	IndexedFace *m_faces; // 24
	AABBNode *m_root; // 28
	uint32_t dword2C; // m_numNodes
	uint32_t dword30; // m_numFaces
	uint32_t dword34; // m_numVertices
	uint32_t dword38; // m_numDegenerateFaces
	uint32_t dword3C; // m_numNonManifoldEdges
	VertexType m_vertexType; // 40
	uint16_t word42; // m_height


	void GetTriangle(MTriangle *tri, IndexedFace *f) const;
};

struct MeshInstance { // line 194
	Matrix m_transformation;
	Vector3 m_streamOffset;
	BBox m_bbox;
	void *ptr70; // m_clientData, m_internalClientData
	Mesh *m_mesh; // 74
	uint16_t m_flags; // 78
	uint16_t dword7A;
	uint32_t dword7C;

	void GetTriangle(MTriangle *tri, IndexedFace *f) const;
	IndexedFace *CollideSegment(
		Vector3& point,
		Vector3& normal,
		float *lambda,
		Vector3Arg start,
		Vector3Arg end,
		uint8_t faceCollideFlags);

	IndexedFace *CollideSweptGeom(
		Vector3& point,
		Vector3& normal,
		float& lambda,
		Vector3Arg start,
		Vector3Arg end,
		uint8_t faceCollideFlags);
};

struct CollisionParams;

bool HandleLeafNode(AABBNode *leaf, CollisionParams *cp);
void Probe(CollisionParams *cp);

}
