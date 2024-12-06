#pragma once
#include <cstdint>

namespace dtp {

struct AnimGraphNode {
	enum NodeTypeEnum : uint32_t {
		NodeTypeEnum_AnimNode = 0x0000,
		NodeTypeEnum_EmbeddedPipe = 0x0001,
		NodeTypeEnum_EmbeddedState = 0x0002,
		NodeTypeEnum_CustomNode = 0x0003
		// NodeTypeEnum_Comment = 0x0004,
	};

	struct Node {
		uint32_t dword0;
		NodeTypeEnum type4;
		void *data;
		void *namedNodeData;
		uint32_t dword10;
	};

	struct Edge {
		uint32_t srcNodeIndex;
		uint32_t srcNodeOutputIndex;
		uint32_t dstNodeIndex;
		uint32_t dstNodeInputIndex;
		uint32_t dword10;
		uint32_t dword14;
	};

	uint32_t numNodes;
	uint32_t numEdges;
	uint32_t numInputs;
	uint32_t numOutputs;
	Node *nodes;
	Edge *edges;
	uint32_t dword18;
};

}
