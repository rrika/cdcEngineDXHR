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

}
