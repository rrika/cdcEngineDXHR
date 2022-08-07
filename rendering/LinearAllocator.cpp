#include "LinearAllocator.h"
#include <windows.h>

namespace cdc {

static void outOfMemory(
	uint32_t howMuch,
	uint32_t requester)
{
	// lol, lmao
}

LinearAllocator::LinearAllocator(
	uint32_t capacity,
	bool allowExtension,
	// AllocatorId *id,
	const char *name)
:
	capacity(capacity ? capacity : 0xA00000),
	allowExtension(allowExtension),
	// id(id),
	name(name)
{
	memory = new char[capacity];
	if (allowExtension) {
		firstDefaultSegment = (Segment*)memory;
		firstDefaultSegment->next = nullptr;
		firstDefaultSegment->prev = nullptr;
		cursor = 16;
	}
}

LinearAllocator::~LinearAllocator() {
	if (allowExtension) {
		auto *seg = firstDefaultSegment;
		while (auto *deleteSeg = seg) {
			seg = deleteSeg->next;
			delete[] (char*)deleteSeg;
		}
		firstDefaultSegment = nullptr;
		memory = nullptr;
		freeBespoke();
	} else {
		delete[] memory;
		memory = nullptr;
	}
	cursor = 0;
	capacity = 0;
}

char *LinearAllocator::allocBespokeSegment(uint32_t size) {
	char *newMemory = new char[size];
	Segment *segment = (Segment*)newMemory;
	segment->next = lastBespokeSegment;
	lastBespokeSegment = segment;
	return (char*)(segment+1);
}

void LinearAllocator::allocDefaultSegment() {
	Segment *oldSegment = (Segment*)memory;

	if (!oldSegment->next) {
		char *newMemory = new char[capacity];
		Segment *newSegment = (Segment*)newMemory;

		oldSegment->next = newSegment;
		newSegment->prev = oldSegment;
		newSegment->next = nullptr;
	}

	memory = (char*)oldSegment->next;
	cursor = 16;
}

char *LinearAllocator::alloc(uint32_t size, uint32_t requester, bool reportFailure) {
	uint32_t size16 = (size + 15) & ~15;

	if (threadSafe) {
		#ifdef _WIN32
		uint32_t initialCursor = InterlockedExchangeAdd(&cursor, size16);
		#else
		// TODO
		uint32_t initialCursor = cursor;
		cursor += size16;
		#endif
		if (initialCursor + size16 > capacity)
			return nullptr;

		return memory + initialCursor;

	} else {
		if (cursor + size16 > capacity) {
			if (!allowExtension) {
				if (reportFailure)
					outOfMemory(size16, requester);
				return nullptr;
			}
			if (size16 > capacity - 16)
				return allocBespokeSegment(size);
			allocDefaultSegment();
		}
		uint32_t initialCursor = cursor;
		cursor += size16;
		return memory + initialCursor;
	}
}

void LinearAllocator::rewind() {
	if (allowExtension) {
		cursor = 16;
		memory = (char*)firstDefaultSegment;
		freeBespoke();
	} else {
		cursor = 0;
	}
}

void LinearAllocator::freeBespoke() {
	while (auto *s = lastBespokeSegment) {
		lastBespokeSegment = s->next;
		delete[] (char*)s;
	}
}

}
