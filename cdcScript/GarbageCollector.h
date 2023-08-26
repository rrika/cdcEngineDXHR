#pragma once
#include <cstdint>

namespace cdc {

union GCNode;
class GCObject;

class GarbageCollector {
	class Impl;

public:
	enum Phase { // 30
		IDLE = 0,
		MARK = 1,
		SWEEP = 2
	};

	static Phase s_phase;
	static GCNode *s_nodes;
	static uint32_t *s_markStack;
	static uint32_t s_markStackTop;
	static bool s_markStackOverflowed;
	static bool s_rootRemoved;
	static GarbageCollector *s_instance;

	static void Create();
	static bool Destroy();

	virtual void Collect() = 0;
	virtual void RigorousCollect() = 0;
	virtual void StartCollection() = 0;
	virtual void Update(int32_t maxObjects) = 0;
	virtual void DestructObject(GCObject*) = 0;
	virtual int32_t NumObjects() = 0;
	virtual void DumpObjects() = 0;
	virtual ~GarbageCollector();
	virtual bool DestroyMembers() = 0;
	virtual void AllocNode(GCObject*, bool markNode) = 0;
};

}
