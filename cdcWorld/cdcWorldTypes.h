#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc { class RenderMesh; class Geom; }

struct SVector { // line 78
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t pad;
};

struct Position { // line 90
	int16_t x;
	int16_t y;
	int16_t z;
};

struct SKUVertex { // line 87
	int16_t x;
	int16_t y;
	int16_t z;
};

struct MVertex { // line 106
	SKUVertex vertex;
	int16_t normal;
	int16_t segment;
};

struct MFace { // line 113
	struct {
		uint16_t v0;
		uint16_t v1;
		uint16_t v2;
	} face;
	uint16_t sameVertBits;
};

struct MJoint { // line 123
	cdc::Vector3 pos;
	cdc::Euler rot;
	int type;
	int body1;
	int body2;
};

struct HPrimFlags { // line 144
	int16_t m_pad1 : 7;
	int16_t m_collide : 1;
	int16_t m_slopeSlide : 1;
	int16_t m_flooring : 1;
	int16_t m_slowSlope : 1;
	int16_t m_noWallJump : 1;
	int16_t m_stairs : 1;
	int16_t m_noRappel : 1;
	int16_t m_wallCrawl : 1;
	int16_t m_noPlayerResponse : 1;
};

struct PhysCollideGameData {
	uint16_t mass;
	char buoyancyFactor;
	char waterCurrentFactor;
	char explosionFactor;
	char weaponhitFactor;
};

struct GameplayGameData {
	int iHitMaterialType;
	int16_t damage;
	char subType;
	char noAttach;
	char nojumpcollision;
	char noWeaponHit;
	char ropeArrowAttach;
	char toughRopePull;
};

struct HSphere {
	HPrimFlags flags;
	char id_or_rank;
	// char id;
	// char rank;
	char indexInSphereList; // 3
	uint16_t radius;
	Position position;
	unsigned int radiusSquared;
	PhysCollideGameData physCollide;
	GameplayGameData gameplay;
};

static_assert(sizeof(HSphere) == 0x24);

struct HBox {
	cdc::Vector3 width; // 0
	cdc::Vector3 pos; // 10
	cdc::Quat quat; // 20
	HPrimFlags flags;
	char id_or_rank;
	// char id;
	// char rank;
	char indexInBoxList; // 23
	PhysCollideGameData physCollide;
	GameplayGameData gameplay;
	uint32_t pad;
	uint32_t padd;
};

static_assert(sizeof(HBox) == 0x50);

struct HGeomCommand {
	HGeomCommand *gc1;
	HGeomCommand *gc2;
	int16_t type;
	int numFloats;
	float *floatList;
	float floatData[];
};

struct HGeom {
	cdc::Vector3 pos;
	cdc::Quat quat;
	cdc::Vector3 centerOfMass;
	float inertiaTensor[9];
	HPrimFlags flags;
	char id_or_rank;
	// char id;
	// char rank;
	char indexInGeomList; // 57
	PhysCollideGameData physCollide;
	GameplayGameData gameplay;
	HGeomCommand *geomCommand;
	cdc::Geom *geom;
};

struct HMarker {
	int segment;
	uint16_t index;
	uint16_t indexInMarkerList;
	float px;
	float py;
	float pz;
	float rx;
	float ry;
	float rz;
};

struct HCapsule {
	cdc::Vector3 pos;
	cdc::Quat quat;
	HPrimFlags flags;
	char id;
	char rank;
	char indexInCapsuleList;
	uint16_t radius;
	uint16_t length;
	PhysCollideGameData physCollide;
	GameplayGameData gameplay;
};

struct HPrim {
	char hpFlags;
	char withFlags;
	int8_t type : 7;
	int8_t bIsOverridden : 1;
	char segment;
	union {
		int none;
		HSphere *hsphere;
		HBox *hbox;
		HMarker *hmarker;
		HCapsule *hcapsule;
		HGeom *hgeom;
	};
};

struct HModel {
	HPrim *hPrimList;
	int numHPrims;
	HMarker *dynamicHMarkerList;
	int numDynamicHMarkers;
};

struct HInfo {
	uint32_t numHSpheres;
	void *hsphereList;
	uint32_t numHBoxes;
	HBox *hboxList;
	uint32_t numHMarkers;
	void *hmarkerList;
	uint32_t numHCapsules;
	void *hcapsuleList;
	uint32_t numHGeoms;
	void *hgeomList;
};

struct Segment {
	uint32_t pad0[8];
	cdc::Vector pivot; // 20
	uint32_t pad30[2];
	uint32_t parent; // 38
	HInfo *hInfo;
};

static_assert(sizeof(Segment) == 0x40);

struct SegmentList { // line 507
	int32_t numSegments; // 0
	Segment *segmentList; // 4
};

namespace dtp {

struct BoneMapEntry {
	uint16_t boneID;
	uint16_t boneIndex;
};

struct BoneMap { // not to be confused with cdc::BoneMap
	uint32_t count;
	uint32_t pad;
	BoneMapEntry *entries;
};

struct Model { // line 516
	void *field_0;
	uint32_t oldNumSegments; // 4
	uint32_t numVirtSegments; // 8
	Segment *oldSegmentList; // C
	void *field_10;
	void *field_14;
	void *field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	void *field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	void *field_44;
	void *field_48;
	void *field_4C;
	void *field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	cdc::RenderMesh *renderMesh; // 64
	SegmentList *segList; // 68
	BoneMap *boneMap; // 6C
	uint32_t boneMapHash; // 70

	inline uint32_t GetNumSegments() {
		if (segList)
			return segList->numSegments;
		else
			return oldNumSegments;
	}
	inline Segment *GetSegmentList() {
		if (segList)
			return segList->segmentList;
		else
			return oldSegmentList;
	}
	inline int16_t GetBoneIndexFromID(uint16_t boneID) {
		if (oldNumSegments == 1)
			return 0;
		// assert (boneMap);
		for (uint32_t i=0; i<boneMap->count; i++) {
			if (boneID == boneMap->entries[i].boneID)
				return boneMap->entries[i].boneIndex;
		}
		return -1;
	}
};

static_assert(sizeof(Model) == 0x74);

}
