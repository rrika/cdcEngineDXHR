#pragma once
#include <cstdint>
#include <cstring> // memcpy
#include "cdcMath/Math.h"

namespace cdc {

struct AABBCollisionDataNode;
struct AABBCollisionTreeNode;

using AABBCollisionCB = bool(AABBCollisionDataNode*, AABBCollisionDataNode*); // line 63

inline uint32_t floatToSortableInt(float f) {
	uint32_t n;
	memcpy(&n, &f, 4);
	if (n < 0x80000000)
		return n | 0x80000000;
	else
		return ~n;
}

struct AABBCollisionNode { // line 71
	uint32_t m_min[3]; // 0
	uint32_t m_max[3]; // C

	void SetBBox(Vector3 min, Vector3 max) {
		m_min[0] = floatToSortableInt(min.x);
		m_min[1] = floatToSortableInt(min.y);
		m_min[2] = floatToSortableInt(min.z);
		m_max[0] = floatToSortableInt(max.x);
		m_max[1] = floatToSortableInt(max.y);
		m_max[2] = floatToSortableInt(max.z);
	}

	bool Overlap(AABBCollisionNode const& other) {
		return m_min[0] < other.m_max[0]
			&& m_max[0] > other.m_min[0]
			&& m_min[1] < other.m_max[1]
			&& m_max[1] > other.m_min[1]
			&& m_min[2] < other.m_max[2]
			&& m_max[2] > other.m_min[2];
	}
};

struct AABBCollisionDataNode : AABBCollisionNode { // line 139
	void *m_client;
	uint16_t m_nextDataOffset; // 1C
	bool m_collideInternal; // 1E

	void SetDataNode(
		AABBCollisionNode const& box, void *client,
		bool collideInternal, uint16_t nextDataOffset);
};

struct AABBCollisionTreeNode : AABBCollisionNode { // line 203
	uint32_t m_rightChild; // 18
	AABBCollisionDataNode *m_dataPtr; // 1C

	uint32_t CollideTree(
		AABBCollisionDataNode **outDataNodePtrs,
		uint32_t maxOutDataNodePtrs,
		AABBCollisionNode const& box,
		AABBCollisionCB *fn);
};

/*
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
*/

struct AABBCollisionJobInstance;

struct AABBCollision { // line 296
	static void AddJobW(AABBCollisionJobInstance *i);
	static void Allocate();
};

struct AABBCollisionJobInstance { // line 353
	// uint32_t dword0; // flags
	// uint32_t dword4; // inTreePtr
	// uint16_t word8; // inMaxTreeNodes
	// uint16_t wordA; // inNumDataNodes
	// uint32_t dwordC; // inDataNodePtrs
	// uint32_t dword10; // inTreePtr2
	// uint32_t dword14; // inNumDataNodes2
	// uint32_t dword18; // inDataNodePtrs2
	// uint32_t dword1C; // inMaxColPairPtrs
	// uint32_t dword20; // outNumTreeNodes
	// uint32_t dword24; // outNextDataNodeOffsets
	// uint32_t dword28; // outNumColPairPtrs
	// uint32_t dword2C; // outColPairPtrs
	void Init() {
		/* TODO */
	}
};
/*
	uint32_t
	cdc::AABBCollisionTreeNode*
	ushort
	ushort
	cdc::AABBCollisionDataNode*
	cdc::AABBCollisionTreeNode*
	uint32_t
	cdc::AABBCollisionDataNode*
	uint32_t
	uint32_t
	short *
	uint32_t
	cdc::AABBCollisionDataNode**
*/

}
