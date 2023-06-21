#pragma once
#include <cstdint>

namespace dtp {

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

	struct Edge {
		// TODO
	};

	uint32_t numStates;
	State *states;
	uint32_t dword8; // numEdges probably
	uint32_t dwordC; // edges probably
	uint32_t primaryOutputIndex;
	bool doInstantiateSubgraphs;
};

}
