#pragma once
#include <cstdint>
#include "../allocator/Allocator.h"

template <typename T>
struct Vector {
	uint32_t size;
	uint32_t capacity;
	union {
		T *data; // when capacity == 0
		cdc::AllocRequester requester; // when capacity != 0
	};

	Vector() : size(0u), capacity(0u), requester(cdc::kAllocArray) {}

	T *begin() { return data; }
	T *end() { return data + size; }
	T const *begin() const { return data; }
	T const *end() const { return data + size; }

	void reserve(uint32_t newCapacity) {
		if (newCapacity > capacity) {
			cdc::AllocRequester req = capacity ? cdc::kAllocMisc : requester;
			auto *newData = new /*(req)*/ T[newCapacity];
			// TOOD: move data over
			if (capacity)
				delete[] data;
			data = newData;
			capacity = newCapacity;
		}
	}
};
