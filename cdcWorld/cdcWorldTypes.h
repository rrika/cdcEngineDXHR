#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc { class RenderMesh; }

struct HInfo;

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
