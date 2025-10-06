#pragma once
#include <vector>
#include "FileSystem.h"

namespace cdc {

class MultiFileSystem : public FileSystem {
public:
	std::vector<FileSystem*> m_systems;

	int32_t ExistsInFileSystem(const char*);
	FileSystem *GetBestFileSystem(const char*);

public:
	MultiFileSystem() {}

	FileRequest *createRequest(FileReceiver*, const char*, uint32_t) override;
	File *createFile(const char*) override;
	bool FileExists(const char*) override;
	uint32_t getSize(const char*) override;
	void setLanguageMask(uint32_t) override;
	uint32_t getLanguageMask() override;
	bool hasRequests() override;
	void processRequest() override;
	void processAll() override;
	void processUntil(FileRequest *request) override;
};

}
