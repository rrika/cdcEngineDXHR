#pragma once
#include "cdcAnim/AnimFragment.h"
#include "cdcAnim/AnimPipelineNode.h"

namespace cdc {

class AnimFragmentNode : public AnimPipelineNode {
	AnimFragment *fragment; // 60

	uint32_t animID; // 6C
	uint32_t boneMapHash; // 70
	BoneMap *boneMap = nullptr; // 74

	bool loop = true;

public:
	void SetAnimData(uint16_t, bool);

public:
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
	void DecompressPose(AnimContextData*);
	void DecompressFrame(AnimContextData*);
};

}
