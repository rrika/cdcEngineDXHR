#include "AABBCollision.h"

namespace cdc {

void AABBCollision::AddJobW(AABBCollisionJobInstance *i) { // line 209
	// TODO
}

void AABBCollision::Allocate() { // line 215
	// TODO
}

void AABBCollisionDataNode::SetDataNode(
	AABBCollisionNode const& box, void *client,
	bool collideInternal, uint16_t nextDataOffset
) {
	*static_cast<AABBCollisionNode*>(this) = box;
	m_client = client;
	m_nextDataOffset = nextDataOffset;
	m_collideInternal = collideInternal;
}


uint32_t AABBCollisionTreeNode::CollideTree( // line 479
	AABBCollisionDataNode **outDataNodePtrs,
	uint32_t maxOutDataNodePtrs,
	AABBCollisionNode const& box,
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
		if (node->m_rightChild) {

			AABBCollisionTreeNode *leftNode = node+1;
			if (leftNode->Overlap(box))
				nodeStack[ns++] = leftNode;

			AABBCollisionTreeNode *rightNode = node+node->m_rightChild;
			if (rightNode->Overlap(box))
				nodeStack[ns++] = rightNode;

		} else {
			if (AABBCollisionDataNode *dnode = node->m_dataPtr) {
				while (true) {
					if (dnode->Overlap(box)) {
						if (outDataNodePtrs && numHits < maxOutDataNodePtrs) {
							outDataNodePtrs[numHits++] = dnode;
						}
						if (fn)
							fn(dnode, nullptr);
					}
					if (dnode->m_nextDataOffset == 0)
						break;
					dnode += dnode->m_nextDataOffset;
				}
			}
		}
	}

	return numHits;
}

}

