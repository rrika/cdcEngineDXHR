#include <cstdio>
#include "cdcMath/MatrixInlines.h"
#include "collideMB.h"
#include "CollideUtils.h"
#include "Mesh.h"

namespace cdc {

class Geom;

FaceCount MeshInstance::Query(FaceIndex *nearFaces, FaceCount maxFaces, BBox const& queryBox, uint8_t faceCollideFlags) { // line 920
	BBox b = queryBox;
	// b.bMin = {b.bMin - m_streamOffset}; // TODO: operator-=
	// b.bMax = {b.bMax - m_streamOffset}; // TODO: operator-=

	printf("%f %f %f %f %f %f\n",
		b.bMin.x,
		b.bMin.y,
		b.bMin.z,
		b.bMax.x,
		b.bMax.y,
		b.bMax.z
	);

	printf("  vs %f %f %f %f %f %f\n",
		m_bbox.bMin.x,
		m_bbox.bMin.y,
		m_bbox.bMin.z,
		m_bbox.bMax.x,
		m_bbox.bMax.y,
		m_bbox.bMax.z
	);

	if (TestAlignedBoxAndAlignedBox(b.bMin, b.bMax, m_bbox.bMin, m_bbox.bMax) == false)
		return 0;

	printf("in\n");

	if (m_flags & 2) {
		Matrix inv;
		OrthonormalInverse3x4(&inv, m_transformation);
		b = CalcXformedBBox(b, inv);

		printf("  tf %f %f %f %f %f %f\n",
			b.bMin.x,
			b.bMin.y,
			b.bMin.z,
			b.bMax.x,
			b.bMax.y,
			b.bMax.z
		);
	}

	uint32_t ns = 0;
	AABBNode *nodeStack[256];
	nodeStack[ns++] = m_mesh->m_root;

	FaceCount numNearFaces = 0;

	while (ns) {
		if (numNearFaces >= maxFaces)
			break;

		AABBNode *node = nodeStack[--ns];
		if (node->m_numFaces > 0) {
			uint16_t limit = node->m_index + node->m_numFaces;
			for (uint16_t i = node->m_index; i < limit; i++) {
				printf(" node %d gives range %d..+%d\n",
					node-m_mesh->m_root, node->m_index, node->m_numFaces);
				if (numNearFaces >= maxFaces)
					break;

				IndexedFace& face = m_mesh->m_faces[i];
				if (face.collisionFlags & faceCollideFlags)
					if (!(face.adjacencyFlags & 0x40))
						nearFaces[numNearFaces++] = i;
			}

		} else {
			AABBNode *lhs = &node[1];
			if (lhs->TestBBox(b))
				nodeStack[ns++] = lhs;

			AABBNode *rhs = &node[node->m_index];
			if (rhs->TestBBox(b))
				nodeStack[ns++] = rhs;
		}
	}

	return numNearFaces;
}


struct CollisionParams { // line 1011
	enum Action {
		COLLIDESEGMENT_ONERESULT = 0x0,
		COLLIDESEGMENT_MANYRESULTS = 0x1,
		COLLIDESWEPTGEOM = 0x2
	};

	Action inAction; // 0
	Vector3 inStart; // 10
	Vector3 inEnd; // 20
	float inLambda; // 30
	Geom const* inGeom; // 34
	MeshInstance *inMeshInstance; // 38
	uint8_t inFaceCollideFlags; // 3C
	SegmentResult *inResults; // 40
	uint32_t inMaxResults; // 44
	uint32_t outNumResults; // 48
	float outLambda; // 4C
	Vector3 outNormal; // 50
	IndexedFace *outFace; // 60
	uint32_t outNumFaceChecks; // 64
	Vector3 startInStreamSpace; // 70
	Vector3 endInStreamSpace; // 74
	Vector3 dirInStreamSpace; // 90
	IndexedFace *facesPtr; // A0
};

bool HandleLeafNode(AABBNode *leaf, CollisionParams *cp) { // line 1064
	if (cp->inAction == CollisionParams::COLLIDESEGMENT_MANYRESULTS && cp->outNumResults >= cp->inMaxResults)
		return false;

	uint8_t numFaces = leaf->m_numFaces;
	FaceIndex firstFace = leaf->m_index;

	float inLambda = cp->inLambda;
	MeshInstance const *mi = cp->inMeshInstance;
	IndexedFace *faces = cp->facesPtr;

	bool hit = false;

	for (unsigned i=0; i < numFaces; i++) {
		FaceIndex iFace = firstFace + i;
		IndexedFace *f = faces + iFace;

		if (f->collisionFlags & cp->inFaceCollideFlags && !(f->adjacencyFlags & 0x40)) {

			cp->outNumFaceChecks++;
			MTriangle tri;
			mi->GetTriangle(&tri, f);
			if (cp->inAction == CollisionParams::COLLIDESEGMENT_ONERESULT) {
				float lambda = 1.f;
				Vector3 normal;
				if (CollideSegmentAndTri(
					&lambda,
					&normal,
					cp->startInStreamSpace,
					cp->dirInStreamSpace,
					tri))
				{
					cp->inResults[cp->outNumResults++] = {f, lambda};
					if (cp->outNumResults == cp->inMaxResults)
						return false;
				}

			} else if (cp->inAction == CollisionParams::COLLIDESEGMENT_MANYRESULTS) {
				// TODO

			} else if (cp->inAction == CollisionParams::COLLIDESWEPTGEOM) {
				// TODO
			}
		}
	}

	return hit;
}

struct NodeSegment { // line 1162
	AABBNode *node;
	float lo;
};

void Probe(CollisionParams *cp) { // line 1192
	float lambda;
	Vector3 normal;
	CollideCode cc;
	auto *mi = cp->inMeshInstance;
	Vector3 start, end, dir;

	bool sweptGeom = cp->inAction == CollisionParams::COLLIDESWEPTGEOM;

	if (sweptGeom) {
		// TODO
	} else {
		cc = CollideSegmentAndAlignedBox(lambda, start, dir, mi->m_bbox);
	}

	if (cc == NO_HIT)
		return;

	if (mi->m_flags & 2) {
		Matrix inv;
		OrthonormalInverse3x4(&inv, mi->m_transformation);
		start = inv * start;
		end = inv * end;
		dir = {end - start};
		if (sweptGeom) {
			// TODO
		}
	}

	if (!sweptGeom) {
		// TODO: calculate 1/dir
	}

	uint32_t ns = 0;
	NodeSegment stack[256];
	stack[ns++] = {mi->m_mesh->m_root, cp->inLambda};

	while (ns) {
		NodeSegment *nodeSegment = &stack[--ns];
		if (lambda < nodeSegment->lo)
			continue;

		if (nodeSegment->node->m_numFaces > 0) {
			if (HandleLeafNode(nodeSegment->node, cp))
				cp->inLambda = cp->outLambda;
			continue;

		} else {
			if (sweptGeom) {
				// TODO
			} else {
				// TODO
			}

		}
	}
}

IndexedFace *MeshInstance::CollideSegment( // line 1475
		Vector3& point,
		Vector3& normal,
		float *lambda,
		Vector3Arg start,
		Vector3Arg end,
		uint8_t faceCollideFlags
) {
	CollisionParams cp = {
		/*Action inAction=*/ CollisionParams::COLLIDESEGMENT_ONERESULT,
		/*Vector3 inStart=*/ start,
		/*Vector3 inEnd=*/ end,
		/*float inLambda=*/ lambda ? *lambda : 1.f,
		/*Geom const* inGeom=*/ nullptr,
		/*MeshInstance *inMeshInstance=*/ this,
		/*uint8_t inFaceCollideFlags=*/ faceCollideFlags,
		/*SegmentResult *inResults=*/ nullptr,
		/*uint32_t inMaxResults=*/ 0, // uninitialized
		/*uint32_t outNumResults=*/ 0,
		/*float outLambda=*/ 0.f,
		/*Vector3 outNormal=*/ {}, // uninitialized
		/*IndexedFace *outFace=*/ nullptr,
		/*uint32_t outNumFaceChecks=*/ 0

		/*Vector3 startInStreamSpace=*/ 
		/*Vector3 endInStreamSpace=*/
		/*Vector3 dirInStreamSpace=*/
		/*IndexedFace *facesPtr=*/
	};

	Probe(&cp);

	if (cp.outFace) {
		point = {start + (start+-end) * cp.outLambda};
		normal = cp.outNormal;
		if (lambda)
			*lambda = cp.outLambda;
	}

	return cp.outFace;
}

IndexedFace *MeshInstance::CollideSweptGeom( // line 1534
	Vector3& point,
	Vector3& normal,
	float& lambda,
	Vector3Arg start,
	Vector3Arg end,
	uint8_t faceCollideFlags
) {
	CollisionParams cp = {
		/*Action inAction=*/ CollisionParams::COLLIDESWEPTGEOM,
		/*Vector3 inStart=*/ start,
		/*Vector3 inEnd=*/ end,
		/*float inLambda=*/ lambda,
		/*Geom const* inGeom=*/ nullptr,
		/*MeshInstance *inMeshInstance=*/ this,
		/*uint8_t inFaceCollideFlags=*/ faceCollideFlags,
		/*SegmentResult *inResults=*/ nullptr,
		/*uint32_t inMaxResults=*/ 0, // uninitialized
		/*uint32_t outNumResults=*/ 0,
		/*float outLambda=*/ 0.f,
		/*Vector3 outNormal=*/ {}, // uninitialized
		/*IndexedFace *outFace=*/ nullptr,
		/*uint32_t outNumFaceChecks=*/ 0

		/*Vector3 startInStreamSpace=*/ 
		/*Vector3 endInStreamSpace=*/
		/*Vector3 dirInStreamSpace=*/
		/*IndexedFace *facesPtr=*/
	};

	Probe(&cp);

	if (cp.outFace) {
		point = {start + (start+-end) * cp.outLambda};
		normal = cp.outNormal;
		lambda = cp.outLambda;
	}

	return cp.outFace;
}

}
