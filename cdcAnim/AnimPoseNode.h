#pragma once
#include "AnimPose.h"
#include "IAnimGraphNode.h"

namespace cdc {

class AnimPoseNode : public IAnimGraphNode {
public:
	IAnimGraphNode *inputNode = nullptr; // 10
	IAnimGraphNode *outputNode = nullptr; // 14
	AnimPose pose; // 18
public:
	AnimPoseNode(AnimComponentV2 *ac, bool addToChain);
	void Activate(Instance*, float) override;
	void Deactivate(Instance*) override;
	float GetNextSyncPoint(void*) override;
	void GetSyncInfo(void*, float *elapsedOut, float *durationOut) override;
	void Update(void*) override;
	void PrePhysics(AnimContextData *data) override;
	IAnimGraphNode *GetOutput(uint32_t index) override;
	uint32_t GetNumOutputs() override;
	void SetOutput(uint32_t index, IAnimGraphNode *node) override;
	IAnimGraphNode *GetInput(uint32_t index) override;
	uint32_t GetNumInputs() override;
	void SetInput(uint32_t index, IAnimGraphNode *node) override;
};

}
