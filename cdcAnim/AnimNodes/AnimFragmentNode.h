#pragma once
#include "cdcAnim/AnimDecoder.h"
#include "cdcAnim/AnimFragment.h"
#include "cdcAnim/AnimPipelineNode.h"
#include "cdc/dtp/animnodes/fragment.h"

namespace cdc {

class AnimComponentV2;

class AnimFragmentNode : public AnimPipelineNode {
	AnimFragment *fragment; // 60

	float elapsedTime;
	uint32_t animID; // 6C
	uint32_t boneMapHash; // 70
	BoneMap *boneMap; // 74
	AnimDecoder *decoders; // 7C

	void SetAnimData(uint16_t);

public:
	AnimFragmentNode(AnimComponentV2 *ac, dtp::FragmentNode *data);
	AnimFragmentNode(AnimComponentV2 *ac, AnimFragment *fragment);
	void Activate(Instance*, float) override;
	void Deactivate(Instance*) override;
	float GetNextSyncPoint(void*) override;
	void GetSyncInfo(void*, float *elapsedOut, float *durationOut) override;
	void Update(void*) override;
	void PrePhysics(AnimContextData*) override;

	// void fun54(...) override;
	// void fun58(...) override;
	// virtual void fun5C(...) { ... } // 5C
	// virtual void fun60(...) { ... } // 60

	void EnsureIDMap(dtp::Model *model);
	void EnsureDecoders();
	void DecompressPose(AnimContextData*);
	void CalcTargetKeyAndTimeOffset(uint32_t& targetKey, float& timeOffset);
	void DecompressFrame(AnimContextData*);
	void DecompressFrame(AnimContextData*, int32_t frameIndex, float fractionalTime);
};

}
