#include <cstdio>
#include <cstring>
#include "GCObject.h"
#include "GarbageCollector.h"
#include "cdcSys/Assert.h"
#include "cdcSys/SystemClock.h"

namespace cdc {

GarbageCollector::Phase GarbageCollector::s_phase;
GCNode *GarbageCollector::s_nodes;
uint32_t *GarbageCollector::s_markStack;
uint32_t GarbageCollector::s_markStackTop;
bool GarbageCollector::s_markStackOverflowed;
bool GarbageCollector::s_rootRemoved;
GarbageCollector *GarbageCollector::s_instance;

GarbageCollector::~GarbageCollector() = default;

class GarbageCollector::Impl : public GarbageCollector {
	GCNode* m_nodes; // 4
	uint32_t m_numNodes; // 8
	uint32_t m_free; // 12, next reclaimable index
	uint32_t m_sweepFree; // 10
	uint32_t m_numAllocated; // 14
	uint32_t m_nodePos; // 18
	bool m_allocatedNodes; // 1C, any nodes allocated during collection
	bool m_remarkNodes; // 1D
	bool m_finalizing; // 1E
	uint32_t m_maxTicks; // 20
	uint32_t m_markStack[512]; // 24

public:
	Impl();
	void GrowNodePool();
	GCNode* GetNextUnmarkedRoot();
	int32_t Mark(int32_t maxObjects, uint32_t ticks);
	int32_t ProcessMarkStack(int32_t, uint32_t);
	int32_t Finalize(int32_t maxObjects, uint32_t ticks);
	void SweepFreeNode(uint32_t);
	int32_t Sweep(int32_t maxObjects, uint32_t ticks);
	void FreeNode(uint32_t);

	void Collect() override; // 0
	void RigorousCollect() override; // 4
	void StartCollection() override; // 8
	void Update(int32_t) override; // C
	void DestructObject(GCObject*) override; // 10
		// called from CollectSequences, DeleteSequence and ~NtSmartScript
	int32_t NumObjects() override; // 14
	void DumpObjects() override; // 18
	~Impl() override = default; // 1C
	bool DestroyMembers() override; // 20
	void AllocNode(GCObject*, bool) override; // 24
		// called by GCObject
};

GarbageCollector::Impl::Impl() {
	// s_gcImpl = this;
	m_numNodes = 0x1C00;
	m_nodes = new GCNode[0x1C00];
	s_nodes = m_nodes;
	memset(m_nodes, 0, sizeof(GCNode[0x1C00]));

	for (uint32_t i=1; i<m_numNodes; i++) {
		m_nodes[i].m_type.allocated = false;
		m_nodes[i].m_free.nextFree = i+1;
	}

	m_free = 1; // first free node
	m_nodes[m_free].m_free.nextAlloc = m_numNodes; // beyond last free node
	m_sweepFree = 0;
	m_numAllocated = 0;
	s_markStackTop = 512;
	s_markStack = m_markStack;
	m_remarkNodes = false;
	m_finalizing = false;
	s_rootRemoved = false;
	s_markStackOverflowed = false;
	s_phase = IDLE;
	m_allocatedNodes = false;
	m_maxTicks = 0x80000000; // 100 * SystemTicksPerMicrosecond();
}

void GarbageCollector::Create() {
	s_instance = new GarbageCollector::Impl();
}

bool GarbageCollector::Destroy() {
	CDC_Assert(s_instance != 0);
	if (s_instance->DestroyMembers()) {
		delete s_instance;
		s_instance = nullptr;
		return true;
	}
	return false;
}

void GarbageCollector::Impl::GrowNodePool() {
	auto numNewNodes = m_numNodes + 1024;
	GCNode *newNodes = new GCNode[numNewNodes];
	memcpy(newNodes, m_nodes, sizeof(GCNode) * m_numNodes);
	memset(newNodes + m_numNodes, 0, sizeof(GCNode) * 1024);
	for (uint32_t i = m_numNodes; i < numNewNodes-1; i++) {
		m_nodes[i].m_type.allocated = false;
		m_nodes[i].m_free.nextFree = i+1;		
	}

	// declare the last 1024 entries all free
	m_free = m_numNodes; // first new node
	m_nodes[m_free].m_free.nextAlloc = numNewNodes; // beyond last new node

	// apply new array
	delete[] m_nodes;
	m_nodes = newNodes;
	s_nodes = newNodes;
	m_numNodes = numNewNodes;
}

GCNode* GarbageCollector::Impl::GetNextUnmarkedRoot() {
	while (m_nodePos != m_numNodes) {
		GCNode *node = &m_nodes[m_nodePos++];
		if (node->m_type.allocated) {
			if (node->m_alloc.rootCount > 0 && !node->m_alloc.mark) {
				node->m_alloc.object->gcmark = true;
				node->m_alloc.mark = true;
				return node;
			}
			if (m_remarkNodes && node->m_alloc.mark)
				return node;
		} else {
			m_nodePos = node->m_free.nextAlloc;
		}
	}
	return nullptr;
}

int32_t GarbageCollector::Impl::Mark(int32_t maxObjects, uint32_t ticks) {
	if (maxObjects <= 0)
		return 0;
	uint32_t objectsProcessed = 0;
	while (objectsProcessed < maxObjects) {
		GCNode *node;
		// all nodes popped?
		if (s_markStackTop == 512) {
			// find something new to do
			node = GetNextUnmarkedRoot();
			if (!node) {
				m_nodePos = 1;
				m_finalizing = true;
				return objectsProcessed;
			}
		} else {
			// pop a node
			uint32_t pos = s_markStack[s_markStackTop++];
			node = &m_nodes[pos];
			if (!node->m_type.allocated)
				continue; // already gone
		}

		if (node->m_alloc.needMark)
			node->m_alloc.object->MarkChildren();

		objectsProcessed++;

		if (GetSystemTicks() - ticks > m_maxTicks)
			return maxObjects; // incorrect, but terminates the outer loop
	}

	return objectsProcessed;
}

int32_t GarbageCollector::Impl::ProcessMarkStack(int32_t maxObjects, uint32_t ticks) {
	if (maxObjects <= 0)
		return 0;
	uint32_t objectsProcessed = 0;
	while (objectsProcessed < maxObjects) {
		// pop a node
		if (s_markStackTop == 512)
			return objectsProcessed;
		uint32_t pos = s_markStack[s_markStackTop++];
		GCNode *node = &m_nodes[pos];
		// if it exists
		if (node->m_type.allocated) {
			// mark it
			if (node->m_alloc.needMark)
				node->m_alloc.object->MarkChildren();

			if (GetSystemTicks() - ticks > m_maxTicks)
				return maxObjects; // incorrect, but terminates the outer loop

			objectsProcessed++;
		}
	}
	return objectsProcessed;
}

int32_t GarbageCollector::Impl::Finalize(int32_t maxObjects, uint32_t ticks) {
	uint32_t timeCheckCounter = 0;
	uint32_t objectsProcessed = 0;
	while (objectsProcessed < maxObjects) {
		if (m_nodePos == m_numNodes)
			break;

		if (s_markStackOverflowed) {
			// redo sweep
			s_markStackOverflowed = false;
			m_finalizing = false;
			m_remarkNodes = true;
			m_nodePos = 1;
			break;
		}

		if (s_markStackTop != 512) {
			// get this out of the way first
			objectsProcessed += ProcessMarkStack(maxObjects - objectsProcessed, ticks);
			continue;
		}

		GCNode *node = &m_nodes[m_nodePos++];
		if (node->m_type.allocated) {
			if (node->m_alloc.mark == false) {
				node->m_alloc.handleNull = true;
				if (node->m_alloc.needFinalize) {
					if (GCObject *object = node->m_alloc.object) {
						object->Finalize();
						timeCheckCounter = 0;
						if (GetSystemTicks() - ticks > m_maxTicks) {
							objectsProcessed = maxObjects;
							break;
						}
					}
				}
				objectsProcessed++;
			}
			if ((++timeCheckCounter) % 32 == 0) {
				if (GetSystemTicks() - ticks > m_maxTicks) {
					objectsProcessed = maxObjects;
					break;
				}
			}

		} else {
			m_nodePos = node->m_free.nextAlloc;
		}
	}

	if (m_nodePos == m_numNodes) {
		// done with marking, time to sweep
		m_remarkNodes = false;
		m_finalizing = false;
		m_nodePos = 1;
		m_sweepFree = 0;
		s_phase = SWEEP;
	}

	return objectsProcessed;
}

void GarbageCollector::Impl::SweepFreeNode(uint32_t index) {
	m_numAllocated--;
	GCNode *node = &m_nodes[index];
	node->m_type.allocated = false;
	uint32_t nextAlloc = index+1;
	// if next slot is also free
	if (nextAlloc < m_numNodes && m_nodes[nextAlloc].m_type.allocated == false)
		// learn about the next alloc from it
		nextAlloc = m_nodes[nextAlloc].m_free.nextAlloc;

	if (m_sweepFree) {
		uint32_t prevFree = m_nodes[m_sweepFree].m_free.nextAlloc-1;
		uint32_t nextFree = m_nodes[prevFree].m_free.nextFree;

		// squeeze between
		node->m_free.nextFree = nextFree;
		node->m_free.nextAlloc = nextAlloc;
		m_nodes[prevFree].m_free.nextFree = index;

		if (index != prevFree+1) {
			// first in its run
			m_sweepFree = index;

		} else {
			// merge into a preceding run of free nodes
			m_nodes[m_sweepFree].m_free.nextAlloc = nextAlloc;
		}

	} else {
		m_sweepFree = index;
		node->m_free.nextFree = m_free;
		node->m_free.nextAlloc = nextAlloc;
		m_free = index;
	}
}

int32_t GarbageCollector::Impl::Sweep(int32_t maxObjects, uint32_t ticks) {
	if (maxObjects <= 0)
		return 0;
	uint32_t timeCheckCounter = 0;
	uint32_t objectsProcessed = 0;
	for (; objectsProcessed < maxObjects && m_nodePos != m_numNodes; objectsProcessed++) {
		GCNode *node = &m_nodes[m_nodePos];
		if (node->m_type.allocated) {
			GCObject *object = node->m_alloc.object;
			if (node->m_alloc.mark) {
				// keep this object, but remove mark
				object->gcmark = false;
				node->m_alloc.mark = false;
				m_nodePos++;
				if ((++timeCheckCounter) % 32 == 0) {
					if (GetSystemTicks() - ticks > m_maxTicks) {
						objectsProcessed = maxObjects;
						break;
					}
				}

			} else {
				if (node->m_alloc.handleCount > 0) {
					// need to keep the node around, but can delete object pointer
					node->m_alloc.object = nullptr;
					m_nodePos++;
				} else {
					// node no longer needed
					SweepFreeNode(m_nodePos);
					m_nodePos = m_nodes[m_nodePos].m_free.nextAlloc;
				}
				if (object)
					delete object;

				timeCheckCounter = 0;
				if (GetSystemTicks() - ticks > m_maxTicks) {
					objectsProcessed = maxObjects;
					break;
				}
			}
		} else {
			if (m_nodes[m_sweepFree].m_free.nextAlloc < m_nodePos)
				m_sweepFree = m_nodePos;
			m_nodePos = node->m_free.nextAlloc;
		}
	}

	if (m_nodePos == m_numNodes) {
		s_phase = IDLE;
		if (s_rootRemoved) {
			s_rootRemoved = false;
			StartCollection();
		}
	}

	return objectsProcessed;
}

void GarbageCollector::Impl::FreeNode(uint32_t index) {
	m_numAllocated--;
	GCNode *node = &m_nodes[index];
	node->m_type.allocated = false;
	uint32_t nextAlloc = index+1;
	// if next slot is also free
	if (nextAlloc < m_numNodes && m_nodes[nextAlloc].m_type.allocated == false)
		// learn about the next alloc from it
		nextAlloc = m_nodes[nextAlloc].m_free.nextAlloc;

	uint32_t sweepIndex;

	// if this is now the lowest free slot
	if (index < m_free || m_free == 0) {
		node->m_free.nextFree = m_free;
		node->m_free.nextAlloc = nextAlloc;
		m_free = index;
		sweepIndex = index;

	} else {
		// find an insertion point
		uint32_t prevFreeHead;      // first of this run
		uint32_t prevFree;          // last of this run
		uint32_t nextFree = m_free; // first of next run
		do {
			prevFreeHead = nextFree;
			prevFree = m_nodes[prevFreeHead].m_free.nextAlloc-1;
			nextFree = m_nodes[prevFree].m_free.nextFree;
		} while (nextFree && nextFree < index);

		// squeeze between
		node->m_free.nextFree = nextFree;
		node->m_free.nextAlloc = nextAlloc;
		m_nodes[prevFree].m_free.nextFree = index;

		if (index != prevFree+1) {
			// first in its run
			sweepIndex = index;

		} else {
			// merge into a preceding run of free nodes
			sweepIndex = prevFreeHead;
			m_nodes[prevFreeHead].m_free.nextAlloc = nextAlloc;
		}
	}
	
	if (s_phase == SWEEP && index <= m_nodePos)
		if (m_sweepFree == 0 || index >= m_sweepFree-1)
			m_sweepFree = sweepIndex;
}

// virtual methods

void GarbageCollector::Impl::Collect() {
	bool collectAgain = s_phase != IDLE;
	StartCollection();
	m_allocatedNodes = false;

	while (s_phase != IDLE) {
		if (s_phase == MARK) {
			if (m_finalizing)
				Finalize(0x7fffffff, GetSystemTicks());
			else
				Mark(0x7fffffff, GetSystemTicks());
		}
		if (s_phase == SWEEP) {
			Sweep(0x7fffffff, GetSystemTicks());
		}

		// restart if needed
		if ((m_allocatedNodes || collectAgain) && s_phase == IDLE) {
			m_allocatedNodes = collectAgain = false;
			StartCollection();
		}
	}
}

void GarbageCollector::Impl::RigorousCollect() {
	uint32_t prevNumAllocated = 0;
	while (m_numAllocated > 0 && m_numAllocated != prevNumAllocated) {
		prevNumAllocated = m_numAllocated;
		Collect();
	}
}

void GarbageCollector::Impl::StartCollection() {
	if (s_phase == IDLE) {
		s_phase = MARK;
		m_nodePos = 1;
	}
}

void GarbageCollector::Impl::Update(int32_t maxObjects) {
	auto startTicks = GetSystemTicks();
	while (s_phase != IDLE) {
		if (maxObjects < 0)
			return;
		if (s_phase == MARK) {
			if (m_finalizing)
				maxObjects -= Finalize(maxObjects, startTicks);
			else
				maxObjects -= Mark(maxObjects, startTicks);
		}
		if (s_phase == SWEEP) {
			maxObjects -= Sweep(maxObjects, startTicks);
		}
	}
}

void GarbageCollector::Impl::DestructObject(GCObject *object) {
	uint32_t nodePos = object->gcnode;
	object->gcmark = false;
	GCNode::AllocNode& node = m_nodes[nodePos].m_alloc;
	node.mark = false;
	node.handleNull = true;
	if (node.needFinalize)
		object->Finalize();
	delete object;
	node.object = nullptr;
	if (node.handleCount == 0)
		FreeNode(nodePos);
}

int32_t GarbageCollector::Impl::NumObjects() {
	return m_numAllocated;
}

void GarbageCollector::Impl::DumpObjects() {
	printf("==========================================================\n");
	printf("GarbageCollector: %d allocated nodes\n", m_numAllocated);
	for (uint32_t i=1; i<m_numNodes; i++) {
		GCNode *node = &m_nodes[i];
		if (node->m_type.allocated) {
			auto& alloc = node->m_alloc;
			if (alloc.object) {
				if (alloc.handleNull) {
					printf("Node %d - Garbage: Roots=%d, Handles=%d, Name=%s\n",
						i, alloc.rootCount, alloc.handleCount, alloc.object->GetGCName());
				} else {
					printf("Node %d - Active:  Roots=%d, Handles=%d, Name=%s\n",
						i, alloc.rootCount, alloc.handleCount, alloc.object->GetGCName());
				}
			} else {
				printf("Node %d - Empty:   Roots=%d, Handles=%d\n",
					i, alloc.rootCount, alloc.handleCount);
			}

		} else {
			// kinda optimistic to rely on the integrity of these nextAlloc
			// indices in a function for diagnosing stuff
			i = node->m_free.nextAlloc;
		}
	}
	printf("==========================================================\n");
}

// GarbageCollector::Impl::~Impl() {
// 	s_gcImpl = nullptr;
// }

bool GarbageCollector::Impl::DestroyMembers() {
	if (m_numAllocated > 0) {
		Collect();
		if (m_numAllocated > 0) {
			DumpObjects();
			return false;
		}
	}

	delete[] m_nodes;
	m_nodes = nullptr;
	s_nodes = nullptr;
	s_markStack = nullptr;
	s_markStackTop = 0;
	return true;
}

void GarbageCollector::Impl::AllocNode(GCObject *obj, bool markNode) {
	if (!m_free)
		GrowNodePool();

	// grab a node
	uint32_t index = m_free;
	GCNode *node = &m_nodes[index];

	// transfer nextAlloc information to adjacent free node
	if (node[0].m_free.nextFree == index+1)
		node[1].m_free.nextAlloc = node[0].m_free.nextAlloc;

	// maintain state
	m_numAllocated++;
	m_allocatedNodes = true;
	m_free = node->m_free.nextFree;

	// set up the node
	node->m_type.allocated = true;
	node->m_alloc.mark = true;
	node->m_alloc.object = obj;
	obj->gcnode = index;

	// determine initial mark
	if (s_phase == IDLE) {
		markNode = false;
	} else if (s_phase == MARK) {
		// leave markNode up to caller
	} else if (s_phase == SWEEP) {
		markNode = index >= m_nodePos;
	}

	// apply initial mark
	node->m_alloc.mark = markNode;
	obj->gcmark = markNode;

	// update m_sweetFree
	if (s_phase == SWEEP && index == m_sweepFree)
		m_sweepFree = m_nodePos >= m_free ? m_free : 0;
}

}
