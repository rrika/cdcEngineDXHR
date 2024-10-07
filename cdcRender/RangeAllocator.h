#pragma once
#include <cstdint>

namespace cdc {

class Allocator;

class RangeAllocator {
public:
	struct Block {
		uint32_t m_start;
		union {
			uint32_t m_padding; // when m_prevFree == nullptr
			Block *m_nextFree;  // when m_prevFree != nullptr
		};
		Block *m_prev = nullptr;
		Block *m_next = nullptr;
		Block *m_prevFree;
	};

	uint32_t m_start;        // 0
	uint32_t m_size;         // 4
	uint32_t m_capacity;     // 8
	uint32_t m_pageSize;     // C
	uint32_t m_firstPage;    // 10
	uint32_t m_minBlockSize; // 14
	Block *m_first;          // 18
	Block *m_last;           // 1C
	Block *m_firstFree;      // 20
	Block *m_pageBounds;     // 24
	Allocator *m_pBlockAllocator; // 28
	uint32_t mode = 0; // 2C

	RangeAllocator(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, Allocator*);
	~RangeAllocator();
	Block *Alloc(uint32_t, uint32_t);
	void Free(Block*);
	void FreeAll();
	void IncreaseSize(uint32_t);
	Block *AllocBlock();
	void FreeBlock(Block*);
	Block *InsertBefore(Block*, uint32_t);
	Block *InsertAfter(Block*, uint32_t);
	void AddFree(Block*);
	void RemoveFree(Block*);
	void SplitIntoPages(Block*);
	Block *AllocExtra(uint32_t, uint32_t);
};

}
