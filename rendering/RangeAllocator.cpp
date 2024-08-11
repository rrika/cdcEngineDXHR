#include "RangeAllocator.h"

namespace cdc {

using Block = RangeAllocator::Block;

RangeAllocator::RangeAllocator(
	uint32_t start, uint32_t size, uint32_t capacity,
	uint32_t pageSize, uint32_t minBlockSize, Allocator *pBlockAllocator)
:
	m_start(start),
	m_size(size),
	m_capacity(capacity),
	m_pageSize(pageSize),
	m_minBlockSize(minBlockSize),
	m_pBlockAllocator(pBlockAllocator)
{
}

Block *RangeAllocator::Alloc(uint32_t size, uint32_t alignment) {
	if (alignment) {
		size += alignment-1;
		size /= alignment;
		size *= alignment;
	}

	uint32_t begin = m_size % m_pageSize;
	uint32_t pageBase = m_size - begin;

	if (alignment) {
		begin += alignment-1;
		begin /= alignment;
		begin *= alignment;
	}

	uint32_t end = begin + size;

	if (end > m_pageSize) {
		// if allocation crosses pages, start from beginning of next page
		pageBase += m_pageSize;
		begin = 0;
		end = size;
	}

	m_size = pageBase + end;

	return new Block {
		pageBase + begin
	};
}

void RangeAllocator::Free(Block *b) {
	delete b;
}

}
