#pragma once
#include <cstdint>
#include "cdcMath/Math.h"

namespace cdc {

struct AABBCollisionDataNode;
struct AABBCollisionTreeNode;

using AABBCollisionCB = bool(AABBCollisionDataNode*, AABBCollisionDataNode*); // line 63

struct AABBCollisionNode { // line 71
	// layout not confirmed
	Vector3 m_min;
	Vector3 m_max;
};

struct AABBCollisionDataNode : AABBCollisionNode { // line 139
	// layout not confirmed
	uint16_t nextIndex;
	uint16_t prevIndex;
	void *m_client;
	AABBCollisionTreeNode *m_treeNode;
	uint16_t m_nextDataOffset;
	uint16_t m_collideInternal : 1;
	uint16_t m_collideBuildTree : 1;
	uint16_t m_buildPairList : 1;
};

struct AABBCollisionTreeNode : AABBCollisionNode { // line 203
	// layout not confirmed
	AABBCollisionTreeNode *next;
	AABBCollisionTreeNode *m_leftChild;
	AABBCollisionTreeNode *m_rightChild;
	AABBCollisionTreeNode *m_parent;
	AABBCollisionDataNode *m_dataPtr;

	uint32_t CollideTree(
		AABBCollisionDataNode **outDataNodePtrs,
		uint32_t maxOutDataNodePtrs,
		AABBCollisionNode& box,
		AABBCollisionCB *fn);
};

}
