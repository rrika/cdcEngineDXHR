#pragma once
#include <cstdio>
#include <cstdint>
#include <memory>
#include <vector>
#include "FileSystem.h"

namespace cdc {

class HackFileSystem;

struct FileWrapper {
	FILE *f;
	FileWrapper(FILE *f) : f(f) {}
	~FileWrapper() { printf("closing %p\n", f); if (f) fclose(f); }
};

class HackFile : public File {
	friend class HackFileSystem;
	HackFileSystem *fs;
	std::shared_ptr<FileWrapper> f;

	HackFile(HackFileSystem *fs, std::shared_ptr<FileWrapper> f) :
		fs(fs), f(f) {}
public:
	FileRequest *createRequest(FileReceiver *receiver, const char *path, uint32_t offset) override;
	uint32_t getSize() override;
};

class HackFileRequest : public FileRequest {
	friend class HackFile;
	friend class HackFileSystem;

	HackFileSystem *fs;
	std::shared_ptr<FileWrapper> f;
	FileReceiver *receiver;
	uint32_t offset;
	uint32_t size = 0;
	uint32_t refCount = 2;

	HackFileRequest(HackFileSystem *fs, std::shared_ptr<FileWrapper> f, FileReceiver *receiver, uint32_t offset) :
		fs(fs), f(f), receiver(receiver), offset(offset) {}

public:
	void incrRefCount() override;
	void decrRefCount() override;
	void setCompressedSize(uint32_t) override;
	void setReadAmount(uint32_t) override;
	void submit(uint8_t) override;
};

class HackFileSystem : public FileSystem {
	friend class HackFileRequest;
	std::vector<HackFileRequest*> requests;
public:
	FileRequest *createRequest(FileReceiver*, const char*, uint32_t) override;
	File *createFile(const char*) override;
	uint32_t getSize(const char*) override;
	bool hasRequests() override;
	void processRequest() override;
};

}
