#pragma once
#include <cstdint>

namespace cdc {

struct IndexedFace;

struct AABBNode { // line 52
	float f_min[3]; // 0
	float f_max[3]; // C
	uint32_t m_numFaces : 8; // 18
	uint32_t m_index : 24; // 1C
};

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
};

/*
struct cdc::MeshInstance
	source: /Volumes/BobSource/tombraider/Companies/Feral/Development/Products/TombRaider/Source/cdc/runtime/cdcCollide/Mesh.h:194
	size: 144
	members:
		0[64]	m_transformation: struct cdc::Matrix
		64[16]	m_streamOffset: struct cdc::Vector3
		80[32]	m_bbox: struct cdc::BBox
		112[4]	m_clientData: * void
		116[4]	m_internalClientData: * void
		120[4]	m_mesh: * struct cdc::Mesh
		124[4]	m_collisionScenePrev: * struct cdc::MeshInstance
		128[4]	m_collisionSceneNext: * struct cdc::MeshInstance
		132[2]	m_flags: cdc::uint16
		134[10]	<padding>

		#pragma pack(push, 1)
struct __declspec(align(2)) cdc_MeshInstance
{
	Matrix4x4 mat0;
	Matrix4x3 mat40;
	void *ptr70;
	cdc_Mesh *cd1;
	_WORD bits;
	_WORD dword7A;
	_DWORD dword7C;
};
#pragma pack(pop)
*/

struct CollisionParams;
void Probe(CollisionParams *cp);

}
