#pragma once
#include "FileSystem.h"

namespace cdc {

class FileSystemLayer : public FileSystem {
protected:
	FileSystem *wrapped;
public:
	FileSystemLayer(FileSystem *wrapped) : wrapped(wrapped) {}

	FileRequest *createRequest(FileReceiver*, const char*, uint32_t) override;
	File *createFile(const char*) override;
	bool hasRequests() override;
	void processRequest() override;
	void processAll() override;
};

}
