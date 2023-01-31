#include <cstring>
#include "RingBufferChain.h"

namespace cdc {

bool RingBufferChain::AddBuffer(char *buffer, uint32_t size) {
	if (m_count == 12)
		return false;

	auto *rb = new RingBuffer();
	rb->m_size = size;
	rb->m_availableBytes = 0;
	rb->buffer = (char*)buffer;
	rb->readCursor = 0;
	rb->writeCursor = 0;
	// rb->dword18 = dword80;

	m_size += size;
	m_posEnd[m_count] = m_size;
	m_ringBuffer[m_count] = rb;
	m_count++;

	return true;
}

char *RingBufferChain::RemoveBuffer() {
	if (m_count == 0)
		return nullptr;

	m_count--;
	auto *rb = m_ringBuffer[m_count];
	m_ringBuffer[m_count] = nullptr;

	if (m_iRead == m_count) {
		m_iRead = 0;
		m_posRead = 0;
	}

	if (m_iWrite == m_count) {
		m_iWrite = 0;
		m_posWrite = 0;
	}

	m_availableBytes -= rb->m_availableBytes;
	m_size = m_count ? m_posEnd[m_count] : 0;

	auto *buffer = rb->buffer;
	delete rb;
	return buffer;
}

void RingBufferChain::push(char *src, uint32_t amount) {
	uint32_t originalAmount = amount;
	bool more = true;

	while (true) {
		uint32_t remainingSpaceInSegment = m_posEnd[m_iWrite] - m_posWrite;
		uint32_t segmentWriteAmount = remainingSpaceInSegment;

		if (amount < remainingSpaceInSegment) {
			segmentWriteAmount = amount;
			more = false;
		}

		m_ringBuffer[m_iWrite]->push(src, segmentWriteAmount);
		m_posWrite += segmentWriteAmount;
		if (src)
			src += segmentWriteAmount;
		amount -= segmentWriteAmount;
		if (!more)
			break;

		if (++m_iWrite == m_count) { // wrap-around
			m_iWrite = 0;
			m_posWrite = 0;
		}

	}
	m_availableBytes += originalAmount;
}

void RingBufferChain::pull(char *dst, uint32_t amount) {
	// TODO
}

}
