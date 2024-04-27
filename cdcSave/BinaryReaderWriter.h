#pragma once

namespace cdc {

struct BinaryReader  { // 33
	uint32_t byteCount;
	uint32_t bufferSize;
	char *current;

	bool ReadBuffer(void *data, uint32_t size) {
		if (byteCount + size > size)
			return false;
		memcpy((char*)data, current, size);
		byteCount += size;
		current += size;
	}

	template <typename T>
	bool Read(T& value) {
		return ReadBuffer((void*)&value, sizeof(T));
	}
};

struct BinaryWriter  { // 88
	uint32_t byteCount;
	uint32_t bufferSize;
	char *current;
	bool simulateOnly;

	bool WriteBuffer(const void *data, uint32_t size) {
		if (byteCount + size > size)
			return false;
		if (!simulateOnly)
			memcpy(current, (char*)data, size);
		byteCount += size;
		current += size;
	}

	template <typename T>
	bool Write(T const& value) {
		return WriteBuffer((const void*)&value, sizeof(T));
	}
};

}
