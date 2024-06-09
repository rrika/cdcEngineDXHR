#pragma once
#include "cdcAnim/AnimFragment.h"
#include "cdcAnim/AnimPipelineNode.h"

namespace cdc {

class AnimComponentV2;
class AnimDecoder;

class AnimFragmentNode : public AnimPipelineNode {
	AnimFragment *fragment; // 60

	float elapsedTime = 0.0f; // 68
	uint32_t animID; // 6C
	uint32_t boneMapHash; // 70
	BoneMap *boneMap = nullptr; // 74
	AnimDecoder *decoders = nullptr; // 7C

	bool loop = true;

public:
	void SetAnimData(uint16_t, bool);

public:
	AnimFragmentNode(AnimComponentV2 *ac, AnimFragment *fragment);
	void Activate(Instance*, float) override;
	void Deactivate(Instance*) override;
	float GetNextSyncPoint(AnimSyncInfoData*) override;
	void GetSyncInfo(AnimSyncInfoData*, float *elapsedOut, float *durationOut) override;
	void Update(AnimUpdateData*) override;
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
	void DecompressFrame(AnimContextData*, uint32_t frameIndex, float fractionalTime);
};

}
