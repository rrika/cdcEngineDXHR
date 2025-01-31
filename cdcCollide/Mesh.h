#pragma once
#include <cstdint>
#include "cdcCollide/collideMB.h"
#include "cdcMath/Math.h"
#include "cdcMath/MatrixInlines.h"

namespace cdc {

struct IndexedFace;

struct AABBNode { // line 52
	float f_min[3]; // 0
	float f_max[3]; // C
	uint16_t m_index; // 18
	uint32_t m_numFaces; // 1A

	bool TestBBox(BBox const& other) const {
		return
			f_max[0] > other.bMin.x &&
			f_max[1] > other.bMin.y &&
			f_max[2] > other.bMin.z &&
			f_min[0] > other.bMax.x &&
			f_min[1] > other.bMax.y &&
			f_min[2] > other.bMax.z;
	}
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

	BBox m_box; // 0
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

	void GetTriangle(MTriangle *tri, IndexedFace *f) const { // line 132
		if (m_vertexType == VERTEX_FLOAT32) {
			tri->v0 = ((Vector3*)vertices)[f->i0];
			tri->v1 = ((Vector3*)vertices)[f->i1];
			tri->v2 = ((Vector3*)vertices)[f->i2];

		} else /* VERTEX_INT16 */ {
			// TODO
		}
	}
};

struct MeshInstance { // line 194
	Matrix m_transformation;
	Vector3 m_streamOffset;
	BBox m_bbox; // post-streamoffset, pre-transformation
	void *ptr70; // m_clientData, m_internalClientData
	Mesh *m_mesh; // 74
	uint16_t m_flags; // 78
	uint16_t dword7A;
	uint32_t dword7C;

	void GetTriangle(MTriangle *tri, IndexedFace *f) const { // line 281
		m_mesh->GetTriangle(tri, f);
		if (m_flags & 2) {
			tri->v0 = m_transformation * tri->v0;
			tri->v1 = m_transformation * tri->v1;
			tri->v2 = m_transformation * tri->v2;
			// TODO: apply m_flags & 1
		}
	}

	FaceCount Query(FaceIndex *nearFaces, FaceCount maxFaces, BBox const& queryBox, uint8_t faceCollideFlags);

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
