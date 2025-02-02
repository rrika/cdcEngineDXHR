#include "collideMB.h"
#include "Mesh.h"

namespace cdc {

int32_t CollideMeshInstanceAndSphere( // line 12
	CPoint *contacts,
	int32_t maxContacts,
	MSphere const& sphere,
	MeshInstance *meshInstance,
	uint8_t faceCollideFlags,
	bool unknown
) {
	Vector r { sphere.radius, sphere.radius, sphere.radius, 0.f };
	BBox bbox { sphere.x - r, sphere.x + r };
	FaceIndex faceIndices[512];
	uint32_t numFaces = meshInstance->Query(faceIndices, 512, bbox, faceCollideFlags);

	Vector3 x = {sphere.x - meshInstance->m_streamOffset};
	uint32_t numContacts = 0;
	IndexedFace *faces = meshInstance->m_mesh->m_faces;
	for (uint32_t i=0; i<numFaces && numContacts<maxContacts; i++) {
		MTriangle mtri;
		IndexedFace& face = faces[faceIndices[i]];
		meshInstance->GetTriangle(&mtri, &face);
		if (CollideTriAndSphere(contacts + numContacts, x, sphere.radius, mtri, face.adjacencyFlags, unknown)) {
			contacts[numContacts].position += meshInstance->m_streamOffset;
			contacts[numContacts].faceIndex = faceIndices[i];
			numContacts++;
		}
	}
	return numContacts;
}

int32_t CollideMeshInstanceAndCapsule( // line 122
	CPoint *contacts,
	int32_t maxContacts,
	MeshInstance *meshInstance,
	Vector3Arg sGlobal,
	Vector3Arg tGlobal,
	float radius,
	BBox aabb,
	uint8_t faceCollideFlags,
	bool unknown
) {
	FaceIndex faceIndices[512];
	uint32_t numFaces = meshInstance->Query(faceIndices, 512, aabb, faceCollideFlags);

	Vector3 s = {sGlobal - meshInstance->m_streamOffset};
	Vector3 t = {tGlobal - meshInstance->m_streamOffset};
	uint32_t numContacts = 0;
	IndexedFace *faces = meshInstance->m_mesh->m_faces;
	for (uint32_t i=0; i<numFaces && numContacts<maxContacts; i++) {
		MTriangle mtri;
		IndexedFace& face = faces[faceIndices[i]];
		meshInstance->GetTriangle(&mtri, &face);
		uint32_t newContacts = CollideTriAndCapsule(contacts + numContacts, maxContacts - numContacts, mtri, s, t, radius, face.adjacencyFlags, unknown);
		for (uint32_t j=0; j<newContacts; j++) {
			contacts[numContacts].position += meshInstance->m_streamOffset;
			contacts[numContacts].faceIndex = faceIndices[i];
			numContacts++;
		}
	}
	return numContacts;
}

}
