#pragma once
#include <cstdint>

namespace cdc {

class FileRequest;
class FileReceiver;

class File {
public:
	virtual FileRequest *createRequest(FileReceiver *receiver, const char *path, uint32_t offset) = 0;
	virtual uint32_t getSize() = 0;
	virtual ~File() = default;
};

class FileRequest {
public:
	virtual void submit() = 0; // 04
	virtual void setCompressedSize(uint32_t) = 0; // 08
	virtual void setReadAmount(uint32_t) = 0; // 0C
	virtual ~FileRequest() = default; // 24
};

class FileSystem {
public:
	virtual FileRequest *createRequest(FileReceiver*, const char*, uint32_t) = 0; // 00
	virtual File *createFile(const char*) = 0; // 04
	virtual uint32_t getSize(const char*) = 0; // 0C
	virtual bool hasRequests() = 0; // 2C
	virtual void processRequest() = 0; // 30
	virtual inline void processAll() { // 34
		while (hasRequests())
			processRequest();
	}
	virtual ~FileSystem() = default; // 60
};

}
