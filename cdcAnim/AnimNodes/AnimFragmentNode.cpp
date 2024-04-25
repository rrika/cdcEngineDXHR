#include <cmath>
#include <cstdio>
#include "AnimFragmentNode.h"
#include "cdcAnim/AnimDecoder.h"
#include "cdcAnim/AnimPose.h"
#include "cdcKit/Animation/anitracker.h"
#include "cdcMath/VectorInlines.h"
#include "cdcSys/Assert.h"
#include "cdcWorld/cdcWorldTypes.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/Object.h"

namespace cdc {

AnimFragmentNode::AnimFragmentNode(AnimComponentV2 *ac, AnimFragment *fragment) :
	AnimPipelineNode(ac, /*numInputs=*/ 0, /*addToChain=*/ true),
	fragment(fragment)
{
	// TODO
}

void AnimFragmentNode::SetAnimData(uint16_t anim, bool loop) {
	// TODO
	if (anim == 0xffff)
		return;
	this->loop = loop;
	auto *object = animComponent->instance->object;
	uint32_t animID = object->animations[anim].animID;
	if (animID) {
		int32_t i = ANITRACKER_FindAnimation(animID);
		fragment = (AnimFragment*)aniTracker[i].animationData;
	} else {
		fragment = nullptr;
	}
	elapsedTime = 0.0f;
}

void AnimFragmentNode::Activate(Instance*, float) {
	// TODO
}

void AnimFragmentNode::Deactivate(Instance*) {
	// TODO
}

float AnimFragmentNode::GetNextSyncPoint(void*) {
	// TODO
	return 0.0f;
}

void AnimFragmentNode::GetSyncInfo(void*, float *elapsedOut, float *durationOut) {
	// TODO
}

void AnimFragmentNode::Update(void*) {
	// TODO
}

void AnimFragmentNode::PrePhysics(AnimContextData *data) {
	if (data->weight >= 0.00001) {
		if (data->weight != 0.0) {

			EnsureDecoders(); // this should happen in Activate, but here is fine too

			if (fragment) { // AnimFragment
				if (fragment->mKeyCount == 1)
					DecompressPose(data);
				else
					DecompressFrame(data);
			} else {
				// TODO
			}
		}
	}

	elapsedTime += 1000/60.f;
	if (elapsedTime >= fragment->mTimePerKey * fragment->mKeyCount)
		elapsedTime -= fragment->mTimePerKey * fragment->mKeyCount;
}

void AnimFragmentNode::EnsureIDMap(dtp::Model *model) {
	// TODO: keep these maps in a hashmap
	if (boneMap &&
		animID == boneMap->animID &&
		boneMapHash == boneMap->boneMapHash)
		return;

	if (boneMap)
		delete boneMap;
	boneMap = GenerateIDMaps(fragment, model);
	
}

void AnimFragmentNode::EnsureDecoders() {
	if (decoders || fragment == nullptr || fragment->mKeyCount == 0)
		return;

	uint32_t decoderCount = 6;
	if (fragment->mExtraChannelCount > 0)
		decoderCount++;

	decoders = new AnimDecoder[decoderCount];
}

void AnimFragmentNode::DecompressPose(AnimContextData *data) {
	EnsureIDMap(data->model);

	BoneSet allBones {1.0f, 0, 249};
	BoneSet *boneSet = data->boneSet;
	if (!boneSet)
		boneSet = &allBones;

	bool cleared = data->pose->clearedByFragment;
	float combinedWeight = boneSet->weight * data->weight;
	uint16_t *masks = fragment->mSegKeyListPtr + 1;
	float *values = fragment->mValueDataPtr + 1;

	for (uint32_t i=0; i<fragment->mSegmentCount; i++) {
		int16_t boneIndex = boneMap->channelToBoneIndex[i];

		float weight;
		Vector3 rot_trans[2];
		Vector3& rot = rot_trans[0];
		Vector3& trans = rot_trans[1];
		if (boneIndex == -1) {
			weight = 0.0f;
		} else {
			if (boneIndex < boneSet->firstBone || boneIndex > boneSet->lastBone) {
				// TODO: boneSet = ...
				combinedWeight = boneSet->weight * data->weight;
			}

			if (fragment->mHasSubtractedFrame) {
				trans = {0.0f, 0.0f, 0.0f, 0.0f};

			} else {
				Segment *segments = data->model->GetSegmentList();
				trans = {segments[boneIndex].pivot};
			}

			rot = {0.0f, 0.0f, 0.0f, 0.0f};
			weight = combinedWeight;
		}

		float *rotc = rot.vec128;
		uint8_t mask = uint8_t(*masks++);
		while (mask) {
			if (mask & 1)
				*rotc = *values++;
			rotc++;
			mask >>= 1;
		}

		if (boneIndex != -1) {
			AnimBuffer *buffer = data->pose->buffer;
			AnimSegment *segments = buffer->segments;
			if (cleared) {
				segments[boneIndex].rot += rot;
				segments[boneIndex].trans += trans;
			} else {
				segments[boneIndex].rot = rot;
				segments[boneIndex].trans = trans;
			}
		}
	}
}

void AnimFragmentNode::CalcTargetKeyAndTimeOffset(uint32_t& targetKey, float& timeOffset) {
	// AnimFragment
	float timePerKey = (float) fragment->mTimePerKey;
	float keyIndexF = elapsedTime / timePerKey;
	uint16_t keyIndex = (uint16_t)keyIndexF;
	uint32_t keyCount = fragment->mKeyCount;
	if (keyIndex >= keyCount)
		keyIndex = keyCount -1;

	float fractionalTime = 0.0f;
	if (elapsedTime >= 0.0f)
		fractionalTime = (elapsedTime - keyIndex * timePerKey) / timePerKey;
	else
		keyIndex = 0;

	targetKey = keyIndex;
	timeOffset = fractionalTime;
}


void AnimFragmentNode::DecompressFrame(AnimContextData *data) {
	uint32_t targetKey;
	float timeOffset;
	CalcTargetKeyAndTimeOffset(targetKey, timeOffset);
	DecompressFrame(data, targetKey, timeOffset);
	data->pose->clearedByFragment = true;
}

void AnimFragmentNode::DecompressFrame(AnimContextData *data, uint32_t keyIndex, float fractionalTime) {
	EnsureIDMap(data->model);

	int16_t keyCount = fragment->mKeyCount;

	int16_t keyA = keyIndex ? keyIndex-1 : 0;
	int16_t keyB = keyIndex >= keyCount ? keyIndex-1 : keyIndex;

	BoneSet allBones {1.0f, 0, 249};
	BoneSet *boneSet = data->boneSet;
	if (!boneSet)
		boneSet = &allBones;

	uint32_t *boneUsage = nullptr;
	// BoneUsageComponent *boneUsageComponent = instance->boneUsageComponent;
	// if (boneUsageComponent) {
	// 	// boneUsage = ...
	// }

	bool cleared = data->pose->clearedByFragment;
	float combinedWeight = boneSet->weight * data->weight;
	uint16_t *masks = fragment->mSegKeyListPtr + 1;
	uint16_t *lengths = fragment->mLengthDataPtr;
	char *values = (char*)(fragment->mValueDataPtr + 1);

	for (uint32_t i=0; i<fragment->mSegmentCount; i++) {
		// BoneMap *boneMap;
		int16_t boneIndex = boneMap->channelToBoneIndex[i];
		bool skip = boneIndex == -1 || (boneUsage /* && bitset check */ );

		float weight;
		Vector3 rot_trans[6];

		rot_trans[0] = {0.0f, 0.0f, 0.0f, 0.0f};
		rot_trans[1] = {0.0f, 0.0f, 0.0f, 0.0f};
		rot_trans[2] = {0.0f, 0.0f, 0.0f, 0.0f};
		rot_trans[3] = {0.0f, 0.0f, 0.0f, 0.0f};
		if (boneIndex != -1 && fragment->mHasSubtractedFrame == false) {
			Segment *segment = &data->model->GetSegmentList()[boneIndex];
			rot_trans[1] = {segment->pivot};
			rot_trans[3] = {segment->pivot};
		}

		if (true || !skip) {
			uint16_t mask = *masks++;
			uint16_t maskCopy = mask;
			uint16_t channelFlags = mask & 0xF00;
			if (channelFlags)
				cdc::FatalError("can't");
			uint32_t componentIndex = 0;
			uint32_t decoderIndex = 0;

			uint32_t numMode1 = 0;
			uint32_t numTotal = 0;

			while (mask) {
				if (mask & 1) {
					uint16_t mode = lengths[0];
					numTotal++;
					if (mode == 0) { // tabulated
						uint16_t fmt16 = lengths[1];
						if (fmt16) {
							auto *offsets = (int16_t*)values;
							rot_trans[0].vec128[componentIndex] = float(offsets[keyA]) / 4096.0f;
							rot_trans[2].vec128[componentIndex] = float(offsets[keyB]) / 4096.0f;
							values += sizeof(uint16_t) * ((fragment->mKeyCount+1) & ~1);
						} else {
							auto *offsets = (float*)values;
							rot_trans[0].vec128[componentIndex] = offsets[keyA];
							rot_trans[2].vec128[componentIndex] = offsets[keyB];
							values += sizeof(float) * fragment->mKeyCount;
						}
						lengths += 2;

					} else if (mode == 1) { // constant
						numMode1++;

						float value = *(float*)values;
						rot_trans[0].vec128[componentIndex] = value;
						rot_trans[2].vec128[componentIndex] = value;

						values += sizeof(float);
						lengths++;

					} else if (mode == 2) { // piece-wise linear
						uint16_t fmt = lengths[1];
						uint16_t numEntries = lengths[2];

						AnimDecoder& dec = decoders[decoderIndex];
						dec.SetChannel(fmt != 0, (char*)values, (uint8_t*)lengths);
						dec.GetValues(fmt != 0, keyA, keyB,
							rot_trans[0].vec128[componentIndex],
							rot_trans[2].vec128[componentIndex]);

						if (fmt) {
							values += sizeof(uint16_t) * ((numEntries+1) & ~1);
						} else {
							values += sizeof(float) * numEntries;
						}
						lengths = (uint16_t*)(uintptr_t(lengths) + 5 + (numEntries|1));
						decoderIndex++;

					} else { // zero
						rot_trans[0].vec128[componentIndex] = 0.0f;
						rot_trans[2].vec128[componentIndex] = 0.0f;
					}
				}

				mask >>= 1;
				channelFlags >>= 1;
				componentIndex++;

				if (channelFlags & 1) {
					mask &= ~channelFlags;
					// SkipComponents(
					// 	&channelFlags,
					// 	&lengths,
					// 	&values,
					// 	keyCount);
				}
			}

			Vector3& rot = rot_trans[4];
			Vector3& trans = rot_trans[5];

			if (numMode1 == numTotal && maskCopy == 0) {
				// everything is constant or uninitialized
				// no tweening necessary
				rot = rot_trans[0];
				trans = rot_trans[1];
			} else {
				const float twoPi = 6.2831855f;
				Vector3& rotA = rot_trans[0];
				Vector3& rotB = rot_trans[2];
				Vector rotDelta = (rotB + - rotA) * (1.f / twoPi);
				float dot = Dot3(rotDelta, rotDelta);
				if (dot > 0.25) {
					if (rotA.IsZero3()) {
						rotA = {rotB.Normalize3() * (1.f / twoPi)};
					} else {
						rotA = rotA - Vector3{rotA.Normalize3()} * (twoPi * floor(sqrtf(dot)+0.5f));
					}
				}

				trans = {rot_trans[1] + (rot_trans[3] + - rot_trans[1]) * fractionalTime};
				rot = {rot_trans[0] + (rot_trans[2] + - rot_trans[0]) * fractionalTime};
			}

			AnimBuffer *buffer = data->pose->buffer;
			AnimSegment *segments = buffer->segments;
			if (skip) {
				// do nothing, but at least the skipping will be correct
			} else if (cleared) {
				segments[boneIndex].rot += rot;
				segments[boneIndex].trans += trans;
			} else {
				segments[boneIndex].rot = rot;
				segments[boneIndex].trans = trans;
			}

		} else {
			// skipping is handled above
			// AdvanceToNextTrack(&masks, &lengths, &values, keyCount);
		}
	}
}

}
