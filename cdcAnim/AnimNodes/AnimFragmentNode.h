#pragma once
#include "../AnimFragment.h"
#include "../AnimPipelineNode.h"

namespace cdc {

class AnimFragmentNode : public AnimPipelineNode {
	// TODO

	void SetAnimData(uint16_t);

public:
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
};

}
