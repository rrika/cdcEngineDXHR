#include "AnimFragmentNode.h"
#include "cdcAnim/AnimDecoder.h"
#include "cdcAnim/AnimPose.h"
#include "cdcKit/Animation/anitracker.h"
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

void AnimFragmentNode::DecompressFrame(AnimContextData *data) {
	EnsureIDMap(data->model);
	// TODO
}

}
