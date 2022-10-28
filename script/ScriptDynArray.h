#pragma once
#include <cstdint>

namespace cdc {

class ScriptDynArrayImpl /* : public GCObject */ {
public:
	// DataType m_type;
	int32_t m_stride = 0;
	int32_t m_size = 0;
	int32_t m_cap = 0;
	uint8_t *m_data = 0;
	// RCPtr<ScriptType> *m_script;

	ScriptDynArrayImpl(int32_t stride) : m_stride(stride) {}
	void Reserve(int32_t size) { m_data = new uint8_t[size * m_stride]; }
	~ScriptDynArrayImpl() { delete[] m_data; }
};

template <typename T>
class ScriptDynArray : public ScriptDynArrayImpl {
public:
	ScriptDynArray(int32_t stride) : ScriptDynArrayImpl (stride) {}
	T const& operator[](int32_t index) const {
		return *(T*)(m_data + m_stride * index);
	}
	T& operator[](int32_t index) {
		return *(T*)(m_data + m_stride * index);
	}
};

template <typename T>
class ScriptNativeDynArray : public ScriptDynArray<T> {
public:
	ScriptNativeDynArray(int32_t stride) : ScriptDynArray<T> (stride) {}
};

}
