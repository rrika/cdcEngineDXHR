#pragma once
#include <cstdint>
#include "GarbageCollector.h"

namespace cdc {

class GCPtrBase;
class GarbageCollector;
void GCMark(GCObject *object);

class GCObject {
	friend class GCPtrBase;
	friend class GarbageCollector;
	friend void GCMark(GCObject *object);
	bool     gcmark : 1;
	uint32_t gcnode : 31;
public:
	GCObject() {
		GarbageCollector::s_instance->AllocNode(this, /*markNode=*/ true);
	}
	virtual ~GCObject();
	virtual void MarkChildren();
	virtual void Finalize();
	virtual const char* GetGCName();
};

union GCNode {
	struct NodeType {
		uint32_t allocated : 1;
	};
	struct FreeNode {
		uint32_t : 1;
		uint32_t nextFree : 31;
		uint32_t nextAlloc : 32;
	};
	struct AllocNode {
		uint32_t : 1;
		uint32_t handleNull : 1;
		uint32_t handleCount : 14;
		uint32_t mark : 1;
		uint32_t needMark : 1;     // object needs MarkChildren call
		uint32_t needFinalize : 1; // object needs Finalize call
		uint32_t rootCount : 13;
		GCObject* object;
	};
	NodeType m_type;
	FreeNode m_free;
	AllocNode m_alloc;
};

void GCMark(GCObject *object) {
	if (object && !object->gcmark) {
		GCNode *node = &GarbageCollector::s_nodes[object->gcnode];
		node->m_alloc.mark = true;
		if (GarbageCollector::s_markStackTop)
			GarbageCollector::s_markStack[--GarbageCollector::s_markStackTop] = object->gcnode;
		else
			GarbageCollector::s_markStackOverflowed = true;
	}
}

template <typename T>
void GCAssign(T *&ptr, T *other) { // line 332
	ptr = other;
	if (other && GarbageCollector::s_phase == GarbageCollector::MARK)
		GCMark(other);
}

// template <typename T>
// void GCAssign(T *&ptr, T *other, GCObject *ptrParent) { // line 346
// 	// TODO
// }

class GCPtrBase {
protected:
	GCObject *target;

	GCPtrBase(GCObject *target) : target(target) {
		if (target && GarbageCollector::s_phase == GarbageCollector::MARK)
			GCMark(target);
		AddReference();
	}
	GCPtrBase() : target(nullptr) {}
	void Assign(GCObject *other) {
		if (target != other) {
			RemReference();
			target = other;
			if (other && GarbageCollector::s_phase == GarbageCollector::MARK)
				GCMark(other);
			AddReference();
		}
	}
	void AddReference() {
		if (target) {
			GCNode *node = &GarbageCollector::s_nodes[target->gcnode];
			node->m_alloc.rootCount++;
		}
	}
	void RemReference() {
		if (target) {
			GCNode *node = &GarbageCollector::s_nodes[target->gcnode];
			if (--node->m_alloc.rootCount == 0 &&
				GarbageCollector::s_phase != GarbageCollector::IDLE)
				GarbageCollector::s_rootRemoved = true;
		}
	}
};

template <typename T>
class GCPtr : public GCPtrBase {
public:
	GCPtr(GCObject *target) : GCPtrBase(target) {}
	GCPtr(GCPtr const& target) : GCPtrBase(target.Get()) {}
	GCPtr() : GCPtrBase() {}
	GCPtr& operator=(T *other) { Assign(other); return *this; }
	GCPtr& operator=(GCPtr const& other) { Assign(other.get()); return *this; }
	T* Get() const { return target; }
	operator T*() const { return target; }
};

}
