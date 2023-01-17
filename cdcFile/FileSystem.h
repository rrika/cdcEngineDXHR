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
	enum Priority {
		INTERRUPT = 0,
		HIGH = 1,
		ABOVE_NORMAL = 2,
		NORMAL = 3,
		BELOW_NORMAL = 4,
		LOW = 5
	};

	virtual void incrRefCount() = 0; // 00
	virtual void decrRefCount() = 0; // 04
	virtual void setCompressedSize(uint32_t) = 0; // 08
	virtual void setReadAmount(uint32_t) = 0; // 0C
	virtual uint32_t getCompletionStatus() = 0; // 14
		// 3 - complete
		// 4 - failed
	virtual void submit(uint8_t priority) = 0; // 18
	virtual void cancel() = 0; // 1C
	virtual ~FileRequest() = default; // 24
};

class FileSystem {
public:
	virtual FileRequest *createRequest(FileReceiver*, const char*, uint32_t) = 0; // 00
	virtual File *createFile(const char*) = 0; // 04
	virtual uint32_t getSize(const char*) = 0; // 0C
	virtual void setLanguageMask(uint32_t) = 0; // 1C
	virtual uint32_t getLanguageMask() = 0; // 20
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
