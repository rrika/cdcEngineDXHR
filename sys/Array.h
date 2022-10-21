#pragma once
#include <cstdint>
#include "../allocator/Allocator.h"

template <typename T>
struct Array {
	uint32_t m_size;
	uint32_t m_capacity;
	union {
		T *m_data; // when m_capacity == 0
		cdc::AllocRequester m_requester; // when m_capacity != 0
	};

	Array() : m_size(0u), m_capacity(0u), m_requester(cdc::kAllocArray) {}

	T *begin() { return m_data; }
	T *end() { return m_data + m_size; }
	T const *begin() const { return m_data; }
	T const *end() const { return m_data + m_size; }
	T *data() { return m_data; }
	T const *data() const { return m_data; }
	uint32_t size() const { return m_size; }

	T& operator[](uint32_t index) { return m_data[index]; }
	T const& operator[](uint32_t index) const { return m_data[index]; }

	void reserve(uint32_t newCapacity) {
		if (newCapacity > m_capacity) {
			cdc::AllocRequester req = m_capacity ? cdc::kAllocMisc : m_requester;
			auto *newData = new /*(req)*/ T[newCapacity];
			// TOOD: move m_data over
			if (m_capacity)
				delete[] m_data;
			m_data = newData;
			m_capacity = newCapacity;
		}
	}
};
