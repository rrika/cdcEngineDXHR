#include "RingBuffer.h"
#include <cstring>

namespace cdc {

void RingBuffer::push(char *src, uint32_t amount) {
	uint32_t firstPart = m_size - writeCursor;
	uint32_t secondPart = amount - firstPart;
	if (amount > firstPart) {
		// wrap-around case
		if (src) {
			memcpy(buffer + writeCursor, src, firstPart);
			memcpy(buffer, src + firstPart, secondPart);
		}
		m_availableBytes += amount;
		writeCursor = secondPart;
	} else {
		// simple case
		if (src)
			memcpy(buffer + writeCursor, src, amount);
		m_availableBytes += amount;
		writeCursor += amount;
		if (writeCursor == m_size)
			writeCursor = 0;
	}
}

void RingBuffer::pull(char *dst, uint32_t amount) {
	uint32_t firstPart = m_size - readCursor;
	uint32_t secondPart = amount - firstPart;
	if (amount > firstPart) {
		// wrap-around case
		if (dst) {
			memcpy(dst, buffer + readCursor, firstPart);
			memcpy(dst + firstPart, buffer, secondPart);
		}
		m_availableBytes -= amount;
		readCursor = secondPart;
	} else {
		// simple case
		if (dst)
			memcpy(dst, buffer + readCursor, amount);
		m_availableBytes -= amount;
		readCursor += amount;
		if (readCursor == m_size)
			readCursor = 0;
	}
}

}
