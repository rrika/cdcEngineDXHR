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
	virtual void incrRefCount() = 0; // 00
	virtual void decrRefCount() = 0; // 04
	virtual void setCompressedSize(uint32_t) = 0; // 08
	virtual void setReadAmount(uint32_t) = 0; // 0C
	virtual uint32_t getCompletionStatus() = 0; // 14
	virtual void submit(uint8_t arg) = 0; // 18
		// 3 - complete
		// 4 - failed
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
	virtual inline void processUntil(FileRequest *request) { // 38
		while (request->getCompletionStatus() != 3)
			processRequest();
	}
	virtual ~FileSystem() = default; // 60
};

}
