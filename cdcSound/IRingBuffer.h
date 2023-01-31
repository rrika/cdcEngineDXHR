#pragma once
#include <cstdint>

namespace cdc {

class DebugIRingBuffer {
protected:
	uint32_t m_size; // 4
	uint32_t m_availableBytes; // 8
};

class IRingBuffer : public DebugIRingBuffer {
public:
	uint32_t getAvailableBytes() { return m_availableBytes; }
	virtual void push(char *src, uint32_t amount) = 0;
	virtual void pull(char *dst, uint32_t amount) = 0;
};

}
