#pragma once
#include <cstdint>

namespace cdc {

class RingBuffer {
	uint32_t size; // 4
	uint32_t availableBytes; // 8
	char *buffer; // C
	uint32_t readCursor; // 10
	uint32_t writeCursor; // 14 
	// _DWORD dword18;
public:
	uint32_t getAvailableBytes() { return availableBytes; }
	char *getBuffer() { return buffer; }
	virtual void push(char *src, uint32_t amount);
	virtual void pull(char *dst, uint32_t amount);
};

}
