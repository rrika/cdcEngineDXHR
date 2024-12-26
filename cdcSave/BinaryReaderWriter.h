#pragma once
#include <cstring>

namespace cdc {

class BinaryReader  { // 33
	uint32_t byteCount;
	uint32_t bufferSize;
	char *current;

	BinaryReader(char *buffer, size_t bufferSize) :
		current(buffer), byteCount(0), bufferSize(bufferSize)
	{}

	bool ReadBuffer(void *data, uint32_t size) {
		if (byteCount + size > size)
			return false;
		memcpy((char*)data, current, size);
		byteCount += size;
		current += size;
		return true;
	}

	template <typename T>
	bool Read(T& value) {
		return ReadBuffer((void*)&value, sizeof(T));
	}
};

class BinaryWriter  { // 88
	uint32_t byteCount;
	uint32_t bufferSize;
	char *current;
public:
	bool simulateOnly = false;

	BinaryWriter(char *buffer, size_t bufferSize) :
		current(buffer), byteCount(0), bufferSize(bufferSize)
	{}

	bool WriteBuffer(const void *data, uint32_t size) {
		if (byteCount + size > size)
			return false;
		if (!simulateOnly)
			memcpy(current, (char*)data, size);
		byteCount += size;
		current += size;
		return true;
	}

	template <typename T>
	bool Write(T const& value) {
		return WriteBuffer((const void*)&value, sizeof(T));
	}
};

}
