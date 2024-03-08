#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace dtp {

struct Model;

}

namespace cdc {

struct AnimSegment {
	Vector3 rot;
	Vector3 trans;
};

struct AnimBuffer {
	AnimSegment *segments = nullptr;
	uint32_t numSegments;
	// Vector3 rootTrans;
	// Quat rootQuatLog;
};

struct BoneSet {
	float weight;
	int16_t firstBone;
	int16_t lastBone;
};

struct BoneMap { // not to be confused with dtp::BoneMap
	uint32_t animID; // from cdc::AnimFragment
	uint32_t boneMapHash; // from dtp::Model
	int16_t channelToBoneIndex[250];
	uint32_t bitset[8];
};

struct AnimFragment {
	char pad[48];
	int16_t mAnimID; // 30
	int16_t mKeyCount; // 32
	int16_t mTimePerKey; // 34
	uint8_t mSegmentCount; // 36
	uint8_t mSectionCount; // 37 (TODO: confirm)
	uint8_t mExtraChannelCount; // 38
	uint32_t dword3C;
	uint32_t mHasSubtractedFrame;
	uint16_t *mExtraChannelLengthPtr; // 44
	float *mExtraChannelDataPtr; // 48
	uint32_t dword4C;
	uint16_t *mBoneIDsDataPtr; // 50
	uint16_t *mSegKeyListPtr; // 54 (component masks)
	uint16_t *mLengthDataPtr; // 58
	float *mValueDataPtr; // 5C
};

static_assert(sizeof(AnimFragment) == 0x60);

BoneMap *GenerateIDMaps(AnimFragment *fragment, dtp::Model *model);

}
