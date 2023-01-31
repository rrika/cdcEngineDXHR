#pragma once
#include <cstdint>
#include "IRingBuffer.h"
#include "RingBuffer.h"

namespace cdc {

class DebugRingBufferChain {
protected:
	RingBuffer *m_ringBuffer[12];
	uint32_t m_posEnd[12];
	uint32_t m_count = 0;
	uint32_t m_iRead = 0;
	uint32_t m_iWrite = 0;
	uint32_t m_posRead = 0;
	uint32_t m_posWrite = 0;
	// uint32_t dword80;
};

class RingBufferChain :
	public IRingBuffer,
	public DebugRingBufferChain
{
public:
	bool AddBuffer(char *buffer, uint32_t size);
	char *RemoveBuffer();

	void push(char *src, uint32_t amount) override;
	void pull(char *dst, uint32_t amount) override;
};

}
