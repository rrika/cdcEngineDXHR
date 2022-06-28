#include "RingBuffer.h"
#include <cstring>

namespace cdc {

void RingBuffer::push(char *src, uint32_t amount) {
	uint32_t firstPart = size - writeCursor;
	uint32_t secondPart = amount - firstPart;
	if (amount > firstPart) {
		// wrap-around case
		if (src) {
			memcpy(buffer + writeCursor, src, firstPart);
			memcpy(buffer, src + firstPart, secondPart);
		}
		availableBytes += amount;
		writeCursor = secondPart;
	} else {
		// simple case
		if (src)
			memcpy(buffer + writeCursor, src, amount);
		availableBytes += amount;
		writeCursor += amount;
		if (writeCursor == size)
			writeCursor = 0;
	}
}

void RingBuffer::pull(char *dst, uint32_t amount) {
	uint32_t firstPart = size - readCursor;
	uint32_t secondPart = amount - firstPart;
	if (amount > firstPart) {
		// wrap-around case
		if (dst) {
			memcpy(dst, buffer + readCursor, firstPart);
			memcpy(dst + firstPart, buffer, secondPart);
		}
		availableBytes -= amount;
		readCursor = secondPart;
	} else {
		// simple case
		if (dst)
			memcpy(dst, buffer + readCursor, amount);
		availableBytes -= amount;
		readCursor += amount;
		if (readCursor == size)
			readCursor = 0;
	}
}

}
