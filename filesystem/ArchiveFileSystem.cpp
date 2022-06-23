#include <algorithm>
#include <cstdio>
#include <cstring>
#include "ArchiveFileSystem.h"
#include "FileUserBufferReceiver.h"

struct ArchiveHeader {
	uint32_t chunkSize;
	char prefix[64];
	uint32_t fileCount;
};

static_assert(sizeof(ArchiveHeader) == 72);

static uint32_t crc32(const char *data, uint32_t length) {
	int32_t hash = -1;

	while (length--) {
		char c = *data++;
		hash = (c << 24) ^ hash;
		for (uint32_t x=0; x<8; x++) {
			if (hash >= 0)
				hash = 2 * hash;
			else
				hash = 2 * hash ^ 0x4C11DB7;
		}
	}
	return (uint32_t)~hash;
}

static uint32_t pathCrc32(const char *path) {
	char buffer[256];
	strcpy(buffer, path);
	strlwr(buffer);
	uint32_t h = crc32(buffer, strlen(buffer));
	return h;
}

static void replaceNumberSuffix(const char *src, char *dst, int i) {
	char suffix[12];
	sprintf(suffix, ".%03d", i);
	strcpy(dst, src);
	if (char *period = strrchr(dst, '.'))
		*period = '\0';
	char *insertionPoint = dst + strlen(dst);
	memcpy(insertionPoint, suffix, strlen(suffix)+1);
}

FileRequest *ArchiveFile::createRequest(FileReceiver *receiver, const char *path, uint32_t offsetWithinFile) {
	return bigfile->createRequest(receiver, path, offset + offsetWithinFile);
}

uint32_t ArchiveFile::getSize() {
	return entry->uncompressedSize;
}

void ArchiveFileSystem::readIndex(const char *basePath, int i) {
	char path[160];
	replaceNumberSuffix(basePath, path, i);
	File *file000 = wrapped->createFile(path);

	ArchiveHeader header;
	auto headerReceiver = FileUserBufferReceiver::create((void*)&header);
	auto headerRequest = wrapped->createRequest(headerReceiver, path, 0);
	headerRequest->setReadAmount(sizeof(ArchiveHeader));
	// headerRequest->method_18(3);
	headerRequest->submit();
	wrapped->processAll();

	if (header.chunkSize)
		chunkSize = header.chunkSize;
	else
		chunkSize = 0x7FF00000;
	strcpy(prefix, header.prefix);
	fileCount = header.fileCount;

	fullIndex = new uint32_t[18 + fileCount * 5]; // sizeof(ArchiveHeader) / 4 + 1*count + 4*count
	auto indexReceiver = FileUserBufferReceiver::create((void*)fullIndex);
	auto indexRequest = wrapped->createRequest(headerReceiver, path, 0);
	headerRequest->setReadAmount(72 + fileCount * 20);
	// headerRequest->method_18(3);
	headerRequest->submit();
	wrapped->processAll();

	hashes = fullIndex + 18;
	entries = (BigfileEntry*)(hashes + fileCount);

	// TODO: compute bigfileCount properly
	bigfileCount = 9;

	bigfiles = new cdc::File*[bigfileCount];
	bigfiles[0] = file000;
	for (uint32_t i = 1; i<bigfileCount; i++) {
		replaceNumberSuffix(basePath, path, i);
		bigfiles[i] = wrapped->createFile(path);
	}
}

BigfileEntry *ArchiveFileSystem::lookupEntry(const char *path) {
	uint32_t hash = pathCrc32(path);
	uint32_t *c = std::lower_bound(hashes, hashes + fileCount, hash);
	if (c == hashes + fileCount)
		return nullptr; // beyond end
	if (*c != hash)
		return nullptr; // no matching entry


	// TODO: scan until fit with language mask

	auto *entry = &entries[c-hashes];
	// printf("found entry: offset=%08x size=%x\n", entry->offset, entry->uncompressedSize);
	return entry;
}

FileRequest *ArchiveFileSystem::createRequest(
	FileReceiver *receiver,
	const char *path,
	uint32_t offsetWithinFile)
{
	if (strnicmp("\\\\local\\", path, 6) == 0)
		return wrapped->createRequest(receiver, path, offsetWithinFile);

	auto entry = lookupEntry(path);
	// if (!entry) cdcError("Can't open file %s\n", path);

	auto offset = uint64_t(entry->offset) << 11;
	auto index = offset / chunkSize;
	auto req = bigfiles[index]->createRequest(receiver, path, (offset % chunkSize) + offsetWithinFile);
	req->setReadAmount(entry->uncompressedSize);
	req->setCompressedSize(entry->compressedSize);
	return req;
}

File *ArchiveFileSystem::createFile(const char *path) {
	if (auto entry = lookupEntry(path)) {
		auto offset = uint64_t(entry->offset) << 11;
		auto index = offset / chunkSize;
		return new ArchiveFile(entry, bigfiles[index], offset % chunkSize);
	}
	return nullptr;
}

uint32_t ArchiveFileSystem::getSize(const char *path) {
	if (strnicmp("\\\\local\\", path, 6) == 0)
		return wrapped->getSize(path);
	else if (auto *entry = lookupEntry(path))
		return entry->uncompressedSize;
	else
		return 0;
}
