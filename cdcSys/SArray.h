#pragma once
#include <cstdint>
#include <cstdlib>

namespace cdc {

template <typename T>
struct SArray {
	T *m_ptr;

	size_t size() { return ((uint32_t*)m_ptr)[-1]; }
	T *begin() { return m_ptr; }
	T *end() { return m_ptr ? m_ptr + size() : m_ptr; }
	T& operator[](size_t index) { return m_ptr[index]; }
	T const& operator[](size_t index) const { return m_ptr[index]; }

	operator bool() const { return m_ptr; }
};

}
