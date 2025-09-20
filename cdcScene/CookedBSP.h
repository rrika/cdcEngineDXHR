#pragma once
#include <cstdint>
#include "cdcRender/Culling/BasicPrimitives.h"

namespace cdc {

struct CookedBSP {
	struct Node {
		struct NodeOrLeaf {
			union {
				Node const *pNode;
				uint32_t nodeIndex;
			};
			uint32_t cellIndex;
		};
		CullingPlane m_plane;
		NodeOrLeaf m_front; // positive side
		NodeOrLeaf m_back; // negative side and along plane
	};
	uint32_t m_numNodes = 0;
	Node *m_pNodes = nullptr;

	void Init(uint32_t numBSPNodes, void *pBSPData); // substitute indices for pointers
};

uint32_t GetCellFromPoint(CookedBSP& bsp, Vector3Arg point);

}
