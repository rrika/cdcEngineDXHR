#pragma once
#include <cstdint>
#include "AnimComponentV2.h"

class Instance;

namespace cdc {

class AnimPose;
struct AnimUpdateData;

struct AnimContextData {
	Instance *instance; // 0
	dtp::Model *model; // 4
	BoneSet *boneSet; // 8
	float weight; // C
	AnimPose *pose; // 10
	// uint8_t byte14;
	// uint8_t byte15;
	// uint16_t word16;
};

class IAnimGraphNode {
protected:
	AnimComponentV2 *animComponent; // 4
	IAnimGraphNode *nextNode = nullptr; // 8
	IAnimGraphNode *prevNode = nullptr; // C
public:
	IAnimGraphNode(AnimComponentV2 *animComponent, bool addToChain) :
		animComponent(animComponent)
	{
		if (addToChain) {
			nextNode = animComponent->firstNode;
			if (nextNode)
				nextNode->prevNode = this;
			animComponent->firstNode = this;
		}
	}

	virtual ~IAnimGraphNode() = default;                              //  0
	virtual void Activate(Instance*, float) = 0;                      //  4
	virtual void Deactivate(Instance*) = 0;                           //  8
	virtual float GetNextSyncPoint(void*) = 0;                        //  C
	virtual void GetSyncInfo(void*, float *elapsedOut, float *durationOut) = 0; // 10
	virtual void Update(AnimUpdateData*) = 0;                         // 14
	virtual void PrePhysics(AnimContextData *data) = 0;               // 18
	virtual IAnimGraphNode *GetOutput(uint32_t index) = 0;            // 1C
	virtual uint32_t GetNumOutputs() = 0;                             // 20
	virtual void SetOutput(uint32_t index, IAnimGraphNode *node) = 0; // 24
	virtual IAnimGraphNode *GetInput(uint32_t index) = 0;             // 28
	virtual uint32_t GetNumInputs() = 0;                              // 2C
	virtual void SetInput(uint32_t index, IAnimGraphNode *node) = 0;  // 30
	// virtual void fun34() = 0;                                      // 34
	// virtual void fun38() = 0;                                      // 38
	// virtual uint8_t fun3C() { return 0; }                          // 3C
	// virtual uint8_t fun40() { return 0; }                          // 40
	// virtual uint8_t fun44() { return 0; }                          // 44
	// virtual uint32_t fun48() { return 0; }                         // 48
	// virtual void fun4C(uint32_t) { /*empty*/ }                     // 4C
	// virtual void fun50() { /*empty*/ }                             // 50
	// virtual void fun54(uint32_t) { /*empty*/ }                     // 54
	// virtual void fun58(uint32_t) { /*empty*/ }                     // 58
};

}
