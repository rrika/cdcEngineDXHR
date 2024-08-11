#pragma once
#include <cstdint>

namespace cdc {

class Allocator;

class RangeAllocator {
public:
	struct Block {
		uint32_t m_start;
		uint32_t m_padding = 0;
	};

	uint32_t m_start;        // 0
	uint32_t m_size;         // 4
	uint32_t m_capacity;     // 8
	uint32_t m_pageSize;     // C
	uint32_t m_firstPage = 0;// 10
	uint32_t m_minBlockSize; // 14
	Allocator *m_pBlockAllocator; // 28

	RangeAllocator(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, Allocator*);

	Block *Alloc(uint32_t, uint32_t);
	void Free(Block*);
};

}
