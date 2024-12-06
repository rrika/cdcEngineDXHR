#pragma once
#include <cstdint>

namespace dtp {

struct TransitionCondition; // animgraphcommon.h

struct AnimStateGraph {
	enum AnimPipeOrStateGraph : uint32_t {
		AnimPipeOrStateGraph_AnimPipe = 0x0000,
		AnimPipeOrStateGraph_StateGraph = 0x0001,
		AnimPipeOrStateGraph_TransientState = 0x0002,
		AnimPipeOrStateGraph_Unknown = 0x0003,
		AnimPipeOrStateGraph_Fragment = 0x0004,
		AnimPipeOrStateGraph_Empty = 0x0005
	};

	struct State {
		uint32_t dword0;
		AnimPipeOrStateGraph type4;
		void *data;
		uint32_t dwordC;
		uint32_t dword10;
	};

	struct Edge { // dtp::AnimStateGraph::AnimStateGraphGraph::Edge
		struct ConditionalAndGroup { // line 126
			TransitionCondition *cond;
			uint32_t count4;
		};

		uint32_t fromIndex;
		uint32_t toIndex;
		uint8_t enabled_maybe8;
		uint32_t dwordC;
		uint16_t trigger_maybe10;
		uint8_t gap12[2];
		uint32_t dword14;
		uint32_t dword18;
		uint32_t dword1C;
		uint32_t dword20;
		uint32_t dword24;
		ConditionalAndGroup *conditionGroups28;
		uint32_t NumConditionGroups2C;
		uint32_t dword30;
		uint32_t dword34;
	};

	uint32_t numStates; // 0
	State *states;      // 4
	uint32_t numEdges;  // 8
	Edge *edges;        // C
	uint32_t primaryOutputIndex;
	bool doInstantiateSubgraphs;
};

}
