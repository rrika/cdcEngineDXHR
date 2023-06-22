#pragma once
#include <cstdint>
#include <vector>
#include "IAnimGraphNode.h"

namespace cdc {

class AnimPipelineNode : public IAnimGraphNode {
	std::vector<IAnimGraphNode*> inputs; // 10
	IAnimGraphNode *output; // 1C
	// uint32_t dword20 = 0;
public:
	AnimPipelineNode(AnimComponentV2*, /*uint32_t,*/ uint32_t numInputs, bool addToChain);

	void Activate(Instance*, float) override;
	void Deactivate(Instance*) override;
	float GetNextSyncPoint(void*) override;
	void GetSyncInfo(void*, float *elapsedOut, float *durationOut) override;
	void Update(void*) override;
	void PrePhysics(AnimContextData *) override;

	IAnimGraphNode *GetOutput(uint32_t) override { return output; }
	uint32_t GetNumOutputs() override { return 1; }
	void SetOutput(uint32_t, IAnimGraphNode *node) override { output = node; }

	IAnimGraphNode *GetInput(uint32_t index) override { return inputs[index]; }
	uint32_t GetNumInputs() override { return inputs.size(); }
	void SetInput(uint32_t index, IAnimGraphNode *node) override { inputs[index] = node; }
};

}
