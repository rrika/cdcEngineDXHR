#pragma once
#include <cstdint>

namespace cdc {

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

}
