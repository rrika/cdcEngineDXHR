#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc {

struct AnimSegment {
	Vector3 rot;
	Vector3 trans;
};

struct AnimBuffer {
	AnimSegment *segments;
	uint32_t numSegments;
	// Vector3 rootTrans;
	// Quat rootQuatLog;
};


/*struct BoneSet {
	float weight;
	int16_t firstBone;
	int16_t lastBone;
};*/

struct AnimFragment {
	char pad[48];
	int16_t mAnimID; // 30 (TODO: confirm)
	int16_t mKeyCount; // 32
	int16_t word34;
	uint8_t mSegmentCount; // 36 (TODO: confirm)
	uint8_t mSectionCount; // 37 (TODO: confirm)
	uint8_t mExtraChannelCount; // 38
	uint32_t dword3C;
	uint32_t dword40;
	uint8_t *mExtraChannelLengthPtr; // 44
	float *mExtraChannelDataPtr; // 48
};

void AdvanceToNextTrack(uint16_t *pChannelFlags, uint8_t *pLengthData, float **pValueData, int32_t keyCount);
void SkipComponents(uint16_t *pChannelFlags, uint8_t *pLengthData, float **pValueData, int32_t keyCount);

}
