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
	m_firstPage(0),
	m_minBlockSize(minBlockSize),
	m_first(nullptr),
	m_last(nullptr),
	m_firstFree(nullptr),
	m_pageBounds(nullptr),
	m_pBlockAllocator(pBlockAllocator)
{
	if (pageSize)
		m_firstPage = start / pageSize;
	FreeAll();
}

RangeAllocator::~RangeAllocator() {
	for (auto *b = m_first; b;) {
		auto *next = b->m_next;
		delete b;
		b = next;
	}
}

Block *RangeAllocator::Alloc(uint32_t size, uint32_t alignment) {
	Block *b = nullptr;
	uint32_t a1 = alignment-1;
	uint32_t pad = 0;
	if (mode == 0) {
		// first fit
		for (auto *free = m_firstFree; free; free = free->m_nextFree) {
			uint32_t start = free->m_start % m_pageSize;
			uint32_t gap = free->m_next->m_start - free->m_start;
			pad = a1 - (start + a1) % alignment;
			if (pad + size <= gap) {
				b = free;
				break;
			}
		}

	} else if (mode == 1) {
		// smallest fit
		uint32_t smallestGap = ~0u;
		for (auto *free = m_firstFree; free; free = free->m_nextFree) {
			uint32_t start = free->m_start % m_pageSize;
			uint32_t gap = free->m_next->m_start - free->m_start;
			uint32_t candidatePad = a1 - (start + a1) % alignment;
			if (candidatePad + size <= gap) {
				if (gap < smallestGap) {
					smallestGap = gap;
					pad = candidatePad;
					b = free;
				}
			}
			if (pad + size == gap)
				break;
		}

	} else
		return nullptr;

	if (!b)
		return AllocExtra(size, alignment);

	RemoveFree(b);

	uint32_t gap = b->m_next->m_start - b->m_start;
	uint32_t newGap = gap - size - pad;
	if (newGap >= m_minBlockSize) {
		auto *gapBlock = InsertAfter(b, b->m_start + pad + size);
		AddFree(gapBlock);
	}

	if (pad < m_minBlockSize) {
		b->m_padding = pad;

	} else {
		auto *padBlock = InsertBefore(b, b->m_start);
		AddFree(padBlock);
		b->m_start += pad;
		b->m_padding = 0;
	}
	return b;
}

void RangeAllocator::Free(Block *b) {
	Block *next = b->m_next;
	Block *prev = b->m_prev;
	uint32_t code = 0;
	if (b != m_first && prev->m_prevFree)
		code |= 1;
	if (next->m_prevFree)
		code |= 2;
	switch (code) {
	case 0:
		// ::: ___ :::
		AddFree(b);
		break;
	case 1:
		// ___ ___ :::
		FreeBlock(b);
		if (m_pageSize)
			SplitIntoPages(prev);
		break;
	case 2:
		// ::: ___ ___
		FreeBlock(b);
		if (m_pageSize)
			SplitIntoPages(next);
		break;
	case 3:
		// ___ ___ ___
		RemoveFree(next);
		FreeBlock(b);
		FreeBlock(next);
		if (m_pageSize)
			SplitIntoPages(prev);
		break;
	}
}

void RangeAllocator::FreeAll() {
	if (m_first) {
		// free all but first node
		auto *it = m_first->m_next;
		while (it) {
			auto *next = it->m_next;
			delete it;
			it = next;
		}
		m_last = m_first;
		// erase only remaining node
		m_first->m_next = nullptr;
		m_first->m_prev = nullptr;
		m_first->m_prevFree = nullptr;
		m_pageBounds = nullptr;
	} else {
		m_first = m_last = AllocBlock();
	}

	m_first->m_start = m_start;
	m_firstFree = nullptr;
	AddFree(m_first);
	InsertAfter(m_first, m_start + m_size);

	if (m_pageSize)
		SplitIntoPages(m_first);
}

void RangeAllocator::IncreaseSize(uint32_t size) {
	if (!m_last->m_prev->m_prevFree)
		AddFree(InsertAfter(m_last->m_prev, m_last->m_start));
	m_size += size;
	m_last->m_start += size;
	if (m_pageSize)
		SplitIntoPages(m_last->m_prev);
}

Block *RangeAllocator::AllocBlock() {
	return new Block {
		.m_start = 0,
		.m_padding = 0,
		.m_prev = nullptr,
		.m_next = nullptr,
		.m_prevFree = nullptr
	};
}

void RangeAllocator::FreeBlock(Block *b) {
	if (b == m_first) {
		m_first = b->m_next;
		m_first->m_prev = nullptr;
	} else
		b->m_prev->m_next = b->m_next;

	if (b == m_last) {
		m_last = b->m_prev;
		m_last->m_next = nullptr;
	} else
		b->m_next->m_prev = b->m_prev;

	delete b;
}

Block *RangeAllocator::InsertBefore(Block *b, uint32_t start) {
	Block *n = new Block{start};
	if (b == m_first) {
		m_first = n;
		n->m_next = b;
		b->m_prev = n;
	} else {
		b->m_prev->m_next = n;
		n->m_prev = b->m_prev;
		n->m_next = b;
		b->m_prev = n;
	}
	return n;
}

Block *RangeAllocator::InsertAfter(Block *b, uint32_t start) {
	Block *n = new Block{start};
	if (b == m_last) {
		m_last = n;
		n->m_prev = b;
		b->m_next = n;
	} else {
		b->m_next->m_prev = n;
		n->m_next = b->m_next;
		n->m_prev = b;
		b->m_next = n;
	}
	return n;
}

void RangeAllocator::AddFree(Block *b) {
	b->m_nextFree = m_firstFree;
	b->m_prevFree = b;
	if (m_firstFree)
		m_firstFree->m_prevFree = b;
	m_firstFree = b;
}

void RangeAllocator::RemoveFree(Block *b) {
	Block *nf = b->m_nextFree;
	if (b == m_firstFree) {
		m_firstFree = nf;
		if (nf)
			nf->m_prevFree = nf;
	} else {
		b->m_prevFree->m_nextFree = nf;
		if (b->m_nextFree)
			b->m_nextFree->m_prevFree = b->m_prevFree;
	}
	b->m_prevFree = nullptr;
	b->m_nextFree = nullptr;
}

void RangeAllocator::SplitIntoPages(Block *b) {
	uint32_t end = b->m_next->m_start;
	if (b->m_start == end) {
		return;
	}

	while (true) {
		end -= (end-1) % m_pageSize + 1;
		if (end <= b->m_start)
			break;

		auto *postBoundary = InsertAfter(b, end);
		AddFree(postBoundary);
		auto *boundary = InsertAfter(b, end);
		if (b == m_last) {
			// never happens	
		} else {
			b->m_next->m_prev = boundary;
			boundary->m_prev = b;
			b->m_next = boundary;
			b->m_nextFree = m_pageBounds;
			m_pageBounds = boundary;
		}
	}
}

// new block goes at the end
Block *RangeAllocator::AllocExtra(uint32_t size, uint32_t alignment) {
	if (m_size == m_capacity)
		return nullptr;
	uint32_t pageSize = m_pageSize;
	uint32_t limit;
	if (pageSize) {
		if (size > m_pageSize)
			return nullptr;
		uint32_t pageCount = (m_start + m_size + pageSize - 1) / pageSize;
		limit = pageCount * pageSize;
	} else {
		pageSize = m_start + m_capacity;
		limit = m_start + m_capacity;
	}

	uint32_t pad, incSize;
	uint32_t a1 = alignment-1;

	Block *last = m_last->m_prev;
	if (last->m_prevFree) { // if last block is free
		pad = a1 - (a1 + last->m_start % pageSize) % alignment;
		uint32_t end = last->m_start + pad + size;
		if (end > m_start + m_capacity)
			return nullptr;
		if (end > limit) {
			incSize = end - m_last->m_start;
			if (incSize)
				goto increase;
		}
	} else {
		pad = a1 - (a1 + (m_start + m_size) % pageSize) % alignment;
		uint32_t end = (m_start + m_size) + pad + size;
		//if (end > m_start + m_capacity)
		if (m_size + pad + size > m_capacity)
			return nullptr;
		if (end > limit) {
			incSize = pad + size;
			if (incSize)
				goto increase;
		}
	}

	if (!m_pageSize)
		return nullptr;
	pad = 0;
	if (limit + size > m_start + m_capacity)
		return nullptr;
	incSize = size + limit - (m_start + m_size);

increase:
	IncreaseSize(incSize);
	last = m_last->m_prev;
	RemoveFree(last);
	if (alignment > 1) {
		if (pad >= m_minBlockSize) {
			AddFree(InsertBefore(last, last->m_start));
			last->m_start += pad;
			last->m_padding = 0;
		} else {
			last->m_padding = pad;
		}
	}
	return last;
}

}
