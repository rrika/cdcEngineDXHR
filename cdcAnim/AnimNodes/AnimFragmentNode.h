#pragma once
#include "cdcAnim/AnimFragment.h"
#include "cdcAnim/AnimPipelineNode.h"
#include "cdc/dtp/animnodes/fragment.h"

namespace cdc {

class AnimComponentV2;

class AnimFragmentNode : public AnimPipelineNode {
	AnimFragment *fragment; // 60

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

	void DecompressPose(AnimContextData*);
	void DecompressFrame(AnimContextData*);
};

}
