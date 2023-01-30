#pragma once
#include <cstdint>

namespace cdc {

// piece-wise linear function
class AnimDecoder {
	char *mOffsetTable;    // 0
	uint8_t *mLengthTable; // 4
	int16_t mLastKey;      // 8
	int16_t mCurKey;       // A
	int32_t mTotal;        // C, sum of lengths
	char *mCurOffset;      // 10
	uint8_t *mCurLength;   // 14
	float mCurBase;        // 18, sum of offsets
	float mCurRangeOffset; // 1C
	float mInvLength;      // 20
	float mLastResult;     // 24
public:
	AnimDecoder();
	void SetChannel(bool fixedPoint, char *pValue, uint8_t *pLength);
	void ResetOffsets(bool fixedPoint);
	void GetValues(bool fixedPoint, int32_t key1, int32_t key2, float& r_value1, float& r_value2);
	float GetValues(bool fixedPoint, int32_t key);
};

}
