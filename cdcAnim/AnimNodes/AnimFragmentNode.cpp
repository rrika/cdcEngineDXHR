#include "AnimFragmentNode.h"
#include "cdcAnim/AnimGraphFactory.h"
#include "cdcAnim/AnimPose.h"
#include "cdcWorld/cdcWorldTypes.h"

namespace cdc {

AnimGraphFactory::Registry<AnimFragmentNode, dtp::FragmentNode>
	s_nodeFactory_AnimDynamicFragmentNode("cdc\\dtp\\animnodes\\Fragment.dtp");

AnimFragmentNode::AnimFragmentNode(AnimComponentV2 *ac, dtp::FragmentNode *data) :
	AnimPipelineNode(ac, /*numInputs=*/ 0, /*addToChain=*/ true)
{
	// TODO
}

AnimFragmentNode::AnimFragmentNode(AnimComponentV2 *ac, AnimFragment *fragment) :
	AnimPipelineNode(ac, /*numInputs=*/ 0, /*addToChain=*/ true),
	fragment(fragment)
{
	// TODO
}

void AnimFragmentNode::SetAnimData(uint16_t anim) {
	// TODO
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
	if (animID == boneMap->animID &&
		boneMapHash == boneMap->boneMapHash)
		return;

	if (boneMap)
		delete boneMap;
	boneMap = GenerateIDMaps(fragment, model);
	
}

void AnimFragmentNode::DecompressPose(AnimContextData *data) {
	EnsureIDMap(data->model);

	BoneSet allBones {1.0f, 0, 249};
	BoneSet *boneSet = data->boneSet;
	if (!boneSet)
		boneSet = &allBones;

	bool additive = false; // data->pose->additive;
	float combinedWeight = boneSet->weight * data->weight;
	uint16_t *masks = fragment->mSegKeyListPtr + 1;
	float *values = fragment->mValueDataPtr + 1;

	for (uint32_t i=0; i<fragment->mSegmentCount; i++) {
		int16_t boneIndex = boneMap->channelToBoneIndex[i];

		float weight;
		Vector3 rot, trans;
		if (boneIndex == -1) {
			weight = 0.0f;
		} else {
			if (boneIndex < boneSet->firstBone || boneIndex > boneSet->lastBone) {
				// TODO: boneSet = ...
				combinedWeight = boneSet->weight * data->weight;
			}

			if (fragment->dword40) {
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

		if (boneIndex == -1) {
			if (additive) {
				// TODO
			} else {
				AnimBuffer *buffer = data->pose->buffer;
				AnimSegment *segments = buffer->segments;
				segments[boneIndex] = {rot, trans};
			}
		}
	}
}

void AnimFragmentNode::DecompressFrame(AnimContextData *data) {
	EnsureIDMap(data->model);
	// TODO
}

}
