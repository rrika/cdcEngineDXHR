#pragma once
#include <cstdint>
#include "ConversationGraph.h"

class ConversationBase;

template <typename Data>
class ConversationGraphNode {
	uint32_t *pdword4;
	uint32_t dword8;
	uint32_t dwordC;
	uint8_t byte10;
	float float14;
	uint32_t dword18;
	// ConversationTier1 *conversation1C;
	uint32_t dword20;
	uint32_t dword24;
	uint32_t dword28;
public:
	ConversationGraphNode(
		ConversationGraph<Data> *graph,
		dtp::ConversationGraphNode *dtpNode,
		ConversationBase*,
		const char *)
	{
		// TODO
	}

	virtual ~ConversationGraphNode() = default;
	virtual void method_4() {};
	// virtual void method_8();
	// virtual void method_C();
	// virtual void method_10();
	// virtual void method_14();
	// virtual void method_18();
	// virtual void method_1C();
	// virtual void method_20();
	// virtual void method_24();
	// virtual void method_28();
};
