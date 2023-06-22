#include <cstring>
#include "AnimFragment.h"
#include "cdcWorld/cdcWorldTypes.h"

namespace cdc {

BoneMap *GenerateIDMaps(AnimFragment *fragment, dtp::Model *model) {
	auto *map = new BoneMap;
	memset(map->channelToBoneIndex, 0, sizeof(map->channelToBoneIndex));
	memset(map->bitset, 0, sizeof(map->bitset));
	for (uint32_t i=0; i<fragment->mSegmentCount; i++) {
		uint16_t boneID = fragment->mBoneIDsDataPtr[i];
		int16_t boneIndex = model->GetBoneIndexFromID(boneID);
		map->channelToBoneIndex[i] = boneIndex;
		if (boneIndex != -1)
			map->bitset[boneIndex >> 5] |= 1 << (boneIndex & 31);
	}
	return map;
}

static const uint32_t s_BitsPerInt[16] = {
	0, 1, 1, 2,
	1, 2, 2, 3,
	1, 2, 2, 3,
	2, 3, 3, 4
};

void AdvanceToNextTrack(uint16_t **pComponentMasks, uint16_t **pLengthData, float **pCompressedFloats, int32_t keyCount) {
	float *compressedFloats; // ecx@1
	uint16_t *sub58; // eax@1
	uint16_t *componentMasks; // edi@1
	int count; // ecx@2
	int mask; // edx@3
	uint16_t *v12; // edi@3
	int v13; // esi@8
	int v14; // edx@8
	bool v15; // dl@12
	int shiftMask; // [sp+1Ch] [bp+Ch]@3

	compressedFloats = *pCompressedFloats;
	sub58 = *pLengthData;
	componentMasks = *pComponentMasks;
	if ( keyCount == 1 )
	{
		count = s_BitsPerInt[*componentMasks & 3]
			  + s_BitsPerInt[((unsigned int)*componentMasks >> 4) & 3]
			  + s_BitsPerInt[((unsigned int)*componentMasks >> 8) & 3];
		*pLengthData = &sub58[count];
		*pCompressedFloats += count;
		*pComponentMasks = componentMasks + 1;
	}
	else
	{                                           // advance through the float by interpreting sub58
												// 0 0   : advance by word32
												// 0 n   : advance by 2 * next_even(word32 + 1)
												// 1     : advance 1
												// 2 0 m : advance m
												// 2 n m x0 x1 x2 ... xm: advance 2 * next_even(m + 1)
												// 
		mask = *componentMasks;
		v12 = componentMasks + 1;
		for ( shiftMask = mask; (uint16_t)shiftMask; shiftMask = (uint16_t)shiftMask >> 1 )
		{
			if ( shiftMask & 1 )
			{
				if ( *sub58 )
				{
					if ( *sub58 == 1 )
					{
						++sub58;
						++compressedFloats;
					}
					else if ( *sub58 == 2 )
					{
						v13 = sub58[2];
						v14 = sub58[1] != 0;
						sub58 = (uint16_t *)((char *)sub58 + v13 + (~(uint8_t)sub58[2] & 1) + 5);
						if ( (uint8_t)v14 )
							compressedFloats = (float *)((char *)compressedFloats + 2 * ((v13 + 1) & 0xFFFFFFFE));
						else
							compressedFloats += v13;
					}
				}
				else
				{
					v15 = sub58[1] != 0;
					sub58 += 2;
					if ( v15 )
						compressedFloats = (float *)((char *)compressedFloats + 2 * ((keyCount + 1) & 0xFFFFFFFE));
					else
						compressedFloats += keyCount;
				}
			}
		}
		*pLengthData = sub58;
		*pCompressedFloats = compressedFloats;
		*pComponentMasks = v12;
	}
}

void SkipComponents(uint16_t *pChannelFlags, uint16_t **pLengthData, float **pValueData, int32_t keyCount) {
	
	int v7;
	bool v8;
	int v9;
	bool v10;
	float **result;

	uint16_t *v4 = *pLengthData;
	uint16_t mask = *pChannelFlags;
	float *compressedFloats = *pValueData;

	for ( mask = *pChannelFlags; mask; mask >>= 1)
	{
		if ( mask & 1 )
		{
			v7 = *v4;
			if ( *v4 )
			{
				if ( v7 == 2 )
				{
					v9 = v4[2];
					v10 = v4[1] != 0;
					v4 = (uint16_t *)((char *)v4 + v9 + (~(uint8_t)v4[2] & 1) + 5);
					if ( v10 )
						compressedFloats = (float *)((char *)compressedFloats + 2 * ((v9 + 1) & 0xFFFFFFFE));
					else
						compressedFloats += v9;
				}
				else if ( v7 == 1 )
				{
					++v4;
					++compressedFloats;
				}
			}
			else
			{
				v8 = v4[1] != (uint16_t)v7;
				v4 += 2;
				if ( v8 )
					compressedFloats = (float *)((char *)compressedFloats + 2 * ((keyCount + 1) & 0xFFFFFFFE));
				else
					compressedFloats += keyCount;
			}
		}
	}
	*pChannelFlags = mask;
	*pLengthData = v4;
	*pValueData = compressedFloats;
}

}
