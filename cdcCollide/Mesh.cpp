#include "Mesh.h"

namespace cdc {

struct CollisionParams { // line 1011
/*
		0[4]	inAction: enum cdc::CollisionParams::Action
		4[12]	<padding>
		16[16]	inStart: struct cdc::Vector3
		32[16]	inEnd: struct cdc::Vector3
		48[4]	inLambda: cdc::float32
		52[4]	inGeom: * const struct cdc::Geom
		56[4]	inMeshInstance: * const struct cdc::MeshInstance
		60[1]	inFaceCollideFlags: cdc::uint8
		61[3]	<padding>
		64[4]	inResults: * struct cdc::SegmentResult
		68[4]	inMaxResults: cdc::uint32
		72[4]	outNumResults: cdc::uint32
		76[4]	outLambda: cdc::float32
		80[16]	outNormal: struct cdc::Vector3
		96[4]	outFace: * struct cdc::IndexedFace
		100[4]	outNumFaceChecks: cdc::uint32
		104[8]	<padding>
		112[16]	outGeomHitPoint: struct cdc::Vector3
		128[16]	startInStreamSpace: struct cdc::Vector3
		144[16]	endInStreamSpace: struct cdc::Vector3
		160[16]	dirInStreamSpace: struct cdc::Vector3
		176[4]	facesPtr: * struct cdc::IndexedFace
		180[12]	<padding>
*/
/*
	int dword0;
	char gap_4[12];
	int var_10;
	int var_14;
	int var_18;
	int var_1C;
	int var_20;
	int var_24;
	int var_28;
	int var_2C;
	float var_30;
	int var_34;
	cdc_MeshInstance *var_38;
	char var_3C;
	char gap_3D[3];
	CTTuple *var_40;
	int lastTriangle;
	int triangleIndex;
	float var_4C;
	int var_50;
	int var_54;
	int var_58;
	int var_5C;
	cdc_IndexedFace *var_60;
	int var_64;
	int var_68;
	int var_6C;
	_DWORD dword70;
	_DWORD dword74;
	_DWORD dword78;
	_DWORD dword7C;
	_DWORD dword80;
	_DWORD dword84;
	_DWORD dword88;
	_DWORD dword8C;
	_DWORD dword90;
	_DWORD dword94;
	_DWORD dword98;
	_DWORD dword9C;
	cdc_IndexedFace *triangles;
*/
};

void Probe(CollisionParams *cp) { // line 1192
	// TODO
}

/*

IndexedFace *MeshInstance::CollideSegment(...) { // line 1475
	// TODO
	// Probe();
}

IndexedFace *MeshInstance::CollideSweptGeom(...) { // line 1534
	// TODO
	// Probe();
}

*/

}
