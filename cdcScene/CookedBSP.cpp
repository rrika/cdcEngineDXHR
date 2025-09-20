#include "CookedBSP.h"
#include "cdcRender/Culling/BasicPrimitives_inlines.h"

namespace cdc {

void CookedBSP::Init(uint32_t numBSPNodes, void *pBSPData) {
	m_numNodes = numBSPNodes;
	m_pNodes = (Node*) pBSPData;

	for (uint32_t i=0; i<m_numNodes; i++) {
		auto& n = m_pNodes[i];
		if (n.m_front.nodeIndex != uint32_t(-1))
			n.m_front.pNode = &m_pNodes[n.m_front.nodeIndex];
		if (n.m_back.nodeIndex != uint32_t(-1))
			n.m_back.pNode = &m_pNodes[n.m_back.nodeIndex];
	}
}

uint32_t GetCellFromPoint(CookedBSP& bsp, Vector3Arg point) {
	CookedBSP::Node const *n = bsp.m_pNodes;
	while (true) {
		CookedBSP::Node::NodeOrLeaf const& child = InFront(n->m_plane, point)
			? n->m_front
			: n->m_back;
		if (uint32_t(child.pNode) == uint32_t(-1)) {
			if (child.cellIndex >> 31 & 1)
				return uint32_t(-1);
			return child.cellIndex;
		}
		if (child.cellIndex == uint32_t(-2))
			return uint32_t(-1);
		n = child.pNode;
	};
}

}
