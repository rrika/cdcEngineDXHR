#pragma once
#include <cstdint>
#include <cdcMath/Math.h>

namespace cdc { class RenderMesh; }

/*struct Segment { // line 445
	0[16]	min: struct cdc::Vector3
	16[16]	max: struct cdc::Vector3
	32[16]	pivot: struct cdc::Vector3
	48[4]	flags: cdc::int32
	52[2]	firstVertex: cdc::int16
	54[2]	lastVertex: cdc::int16
	56[4]	parent: cdc::int32
	60[4]	hInfo: * struct HInfo
};*/

struct HInfo {
	uint32_t numHSpheres;
	void *hsphereList;
	uint32_t numHBoxes;
	void *hboxList;
	uint32_t numHMarkers;
	void *hmarkerList;
	uint32_t numHCapsules;
	void *hcapsuleList;
	uint32_t numHGeoms;
	void *hgeomList;
};

struct Segment {
	uint32_t pad[14];
	uint32_t parent;
	HInfo *hInfo;
};

struct SegmentList { // line 507
	int32_t numSegments; // 0
	Segment *segmentList; // 4
};

namespace dtp {

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
	uint32_t *pdword6C;
	uint32_t dword70;

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
};

static_assert(sizeof(Model) == 0x74);

}
