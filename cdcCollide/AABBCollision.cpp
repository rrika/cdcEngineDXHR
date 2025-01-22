#include "AABBCollision.h"

namespace cdc {

void AABBCollision::AddJobW(AABBCollisionJobInstance *i) { // line 209
	// TODO
}

void AABBCollision::Allocate() { // line 215
	// TODO
}

uint32_t AABBCollisionTreeNode::CollideTree( // line 479
	AABBCollisionDataNode **outDataNodePtrs,
	uint32_t maxOutDataNodePtrs,
	AABBCollisionNode& box,
	AABBCollisionCB *fn
) {
	if (!Overlap(box))
		return 0;

	uint32_t ns = 0;
	AABBCollisionTreeNode *nodeStack[256];
	nodeStack[ns++] = this;

	uint32_t numHits = 0;

	while (ns) {
		AABBCollisionTreeNode *node = nodeStack[--ns];
		if (node->m_rightIndex) {

			AABBCollisionTreeNode *leftNode = node+1;
			if (leftNode->Overlap(box))
				nodeStack[ns++] = leftNode;

			AABBCollisionTreeNode *rightNode = node+node->m_rightIndex;
			if (rightNode->Overlap(box))
				nodeStack[ns++] = rightNode;

		} else {
			if (AABBCollisionDataNode *node = node->m_dataPtr) {
				while (true) {
					if (node->Overlap(box)) {
						if (outDataNodePtrs && numHits < maxOutDataNodePtrs) {
							outDataNodePtrs[numHits++] = node;
						}
						if (fn)
							fn(node, nullptr);
					}
					if (node->m_nextDataOffset == 0)
						break;
					node += node->m_nextDataOffset;
				}
			}
		}
	}

	return numHits;
}

}

