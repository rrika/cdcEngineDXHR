#pragma once
#include <cstdint>
#include "IRingBuffer.h"

namespace cdc {

class DebugRingBuffer {
protected:
	char *buffer; // C
	uint32_t readCursor; // 10
	uint32_t writeCursor; // 14 
	// _DWORD dword18;
};

class RingBuffer :
	public IRingBuffer,
	public DebugRingBuffer
{
public:
	char *getBuffer() { return buffer; }
	void push(char *src, uint32_t amount) override;
	void pull(char *dst, uint32_t amount) override;
};

}
