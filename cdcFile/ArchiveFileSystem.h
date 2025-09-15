#pragma once
#include "FileSystem.h"
#include "FileSystemLayer.h"

struct BigfileEntry {
	uint32_t uncompressedSize;
	uint32_t offset;
	uint32_t language;
	uint32_t compressedSize;
};

class ArchiveFileSystem;

class ArchiveFile : public cdc::File {
	BigfileEntry *entry;
	cdc::File *bigfile;
	uint32_t offset;

	friend class ArchiveFileSystem;
	ArchiveFile(BigfileEntry *entry, File *bigfile, uint32_t offset) :
		entry(entry), bigfile(bigfile), offset(offset) {}
public:
	cdc::FileRequest *createRequest(cdc::FileReceiver *receiver, const char *path, uint32_t offset) override;
	uint32_t getSize() override;
};

class ArchiveFileSystem : public cdc::FileSystemLayer {
protected:
	uint32_t languageMask = 0xBFFF0001; // DX11 English

	cdc::File **bigfiles = nullptr;
	uint32_t bigfileCount;
	uint32_t chunkSize;

	BigfileEntry *entries;
	uint32_t *hashes;
	uint32_t *fullIndex;

	uint32_t fileCount;
	char prefix[64];
public:
	ArchiveFileSystem(cdc::FileSystem *wrapped) : FileSystemLayer(wrapped) {}
	bool readIndex(const char *basePath, int i);
	BigfileEntry *lookupEntry(const char *path);
	const char *getPrefix();

	cdc::FileRequest *createRequest(cdc::FileReceiver *receiver, const char *path, uint32_t offset) override;
	cdc::File *createFile(const char *path) override;
	bool FileExists(const char *path) override;
	uint32_t getSize(const char *path) override;
	void setLanguageMask(uint32_t) override;
	uint32_t getLanguageMask() override;
};
