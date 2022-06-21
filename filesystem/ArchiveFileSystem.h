#pragma once
#include "FileSystem.h"
#include "FileSystemLayer.h"

using namespace cdc;

struct BigfileEntry {
	uint32_t uncompressedSize;
	uint32_t offset;
	uint32_t language;
	uint32_t compressedSize;
};

class ArchiveFileSystem;

class ArchiveFile : public cdc::File {
	BigfileEntry *entry;
	File *bigfile;
	uint32_t offset;

	friend class ArchiveFileSystem;
	ArchiveFile(BigfileEntry *entry, File *bigfile, uint32_t offset) :
		entry(entry), bigfile(bigfile), offset(offset) {}
public:
	FileRequest *createRequest(FileReceiver *receiver, const char *path, uint32_t offset) override;
	uint32_t getSize() override;
};

class ArchiveFileSystem : public cdc::FileSystemLayer {
protected:
	File **bigfiles = nullptr;
	uint32_t bigfileCount;
	uint32_t chunkSize;

	BigfileEntry *entries;
	uint32_t *hashes;
	uint32_t *fullIndex;

	uint32_t fileCount;
	char prefix[64];
public:
	ArchiveFileSystem(FileSystem *wrapped) : FileSystemLayer(wrapped) {}
	void readIndex(const char *basePath, int i);
	BigfileEntry *lookupEntry(const char *path);

	FileRequest *createRequest(FileReceiver *receiver, const char *path, uint32_t offset) override;
	File *createFile(const char *path) override;
};
