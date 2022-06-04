#pragma once
#include <cstdint>

namespace cdc {

struct AllocatorId;

class LinearAllocator {
	struct Segment {
		// for default segments "next" points to newer segments
		// for bespoke segments "next" points to older segments
		Segment *next;
		Segment *prev;
		uint32_t dword8;
		uint32_t dwordC;
	};
	char *memory = nullptr;  //  0
	uint32_t cursor = 0;     //  4
	uint32_t capacity;       //  8
	bool threadSafe = false; //  C
	uint32_t dword10 = 0;    // 10
	Segment *lastBespokeSegment = nullptr; // 14
	bool allowExtension;     // 18
	// uint32_t dword1C;     // 1C
	AllocatorId *id;         // 20
	const char *name;        // 24
	Segment *firstDefaultSegment = nullptr; // 28

	char *allocBespokeSegment(uint32_t size);
	void allocDefaultSegment();
	void freeBespoke();

public:
	LinearAllocator(
		uint32_t capacity,
		bool allowExtension,
		// uint32_t unknown,
		// AllocatorId *id,
		const char *name
		// const char **categories
	);
	~LinearAllocator();

	char *alloc(uint32_t size, uint32_t requester=0, bool reportFailure=false);
	void rewind();
};

};

inline void *operator new(size_t count, cdc::LinearAllocator *linear, uint32_t requester=0, bool reportFailure=false) {
	return (void*)linear->alloc((uint32_t)count, requester, reportFailure);
}
