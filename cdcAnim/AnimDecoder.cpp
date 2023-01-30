#include "AnimDecoder.h"

namespace cdc {

AnimDecoder::AnimDecoder() :
	mOffsetTable(nullptr),
	mLengthTable(nullptr),
	// mLastKey(),
	mCurKey(0),
	mTotal(0),
	mCurOffset(nullptr),
	mCurLength(nullptr),
	mCurBase(0.0f),
	mCurRangeOffset(0.0f),
	mInvLength(1.0f)
	// mLastResult()
{}

void AnimDecoder::SetChannel(bool fixedPoint, char *pValue, uint8_t *pLength) {
	uint8_t *pLength6 = pLength + 6;
	if (pValue != mOffsetTable || pLength6 != mLengthTable) {

		mLengthTable = pLength6;
		mOffsetTable = pValue;
		mLastKey = -1;
		mCurKey = -1;
		mCurRangeOffset = 0.0f;
		mLastResult = 0.0f;

		ResetOffsets(fixedPoint);
	}
}

void AnimDecoder::ResetOffsets(bool fixedPoint) {

	if (fixedPoint) {
		mCurBase = *(int16_t*)mOffsetTable / 4096.0;
		mCurOffset = mOffsetTable + 2;

	} else {
		mCurBase = *(float*)mOffsetTable;
		mCurOffset = mOffsetTable + 4;
	}

	mTotal = 0;
	mCurLength = mLengthTable;
	mInvLength = 1.0 / *mLengthTable;
}

void AnimDecoder::GetValues(bool fixedPoint, int32_t key1, int32_t key2, float& r_value1, float& r_value2) {
	r_value1 = GetValues(fixedPoint, key1);
	r_value2 = GetValues(fixedPoint, key2);
}

float AnimDecoder::GetValues(bool fixedPoint, int32_t targetKey) {

	if (targetKey == mCurKey)
		return mCurBase + mCurRangeOffset;

	if (targetKey == mLastKey)
		return mLastResult;

	mLastResult = mCurBase + mCurRangeOffset;
	mLastKey = mCurKey;
	if (targetKey < mCurKey) // need rewind?
		ResetOffsets(fixedPoint);

	mCurKey = targetKey;

	uint8_t *pLength = mCurLength;
	uint32_t total = mTotal;

	if (fixedPoint) {
		int32_t offset = 0;
		int16_t *pOffset = (int16_t*)mCurOffset;

		if (targetKey - total > *pLength) {

			do { // skip complete segment
				total += *pLength++;
				offset += *pOffset++;
			} while (targetKey - total > *pLength);

			mCurOffset = (char*)pOffset;
			mCurLength = pLength;
			mCurBase += (offset / 4096.0); // sum of offsets
			mTotal = total; // sum of lengths
			mInvLength = 1.0 / *mCurLength;
		}

		mCurRangeOffset = (targetKey - total) * (*pOffset / 4096.0) * mInvLength;
		return mCurBase + mCurRangeOffset;

	} else {
		float base = mCurBase;
		float *pOffset = (float*)mCurOffset;

		if (targetKey - total > *pLength) {

			do { // skip complete segment
				total += *pLength++;
				base += *pOffset++;
			} while (targetKey - total > *pLength);

			mCurOffset = (char*)pOffset;
			mCurLength = pLength;
			mCurBase = base; // sum of offsets
			mTotal = total; // sum of lengths
			mInvLength = 1.0 / *mCurLength;
		}

		mCurRangeOffset = (targetKey - total) * *pOffset * mInvLength;
		return base + mCurRangeOffset;
	}
}

}
