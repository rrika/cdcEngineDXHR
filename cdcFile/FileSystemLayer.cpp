#include "FileSystemLayer.h"

namespace cdc {

FileRequest *FileSystemLayer::createRequest(FileReceiver *receiver, const char *path, uint32_t offset) {
	return wrapped->createRequest(receiver, path, offset);
}

File *FileSystemLayer::createFile(const char *path) {
	return wrapped->createFile(path);
}

bool FileSystemLayer::FileExists(const char *path) {
	return wrapped->FileExists(path);
}

uint32_t FileSystemLayer::getSize(const char *path) {
	return wrapped->getSize(path);
}

void FileSystemLayer::setLanguageMask(uint32_t mask) {
	return wrapped->setLanguageMask(mask);
}

uint32_t FileSystemLayer::getLanguageMask() {
	return wrapped->getLanguageMask();
}

bool FileSystemLayer::hasRequests() {
	return wrapped->hasRequests();
}

void FileSystemLayer::processRequest() {
	wrapped->processRequest();
}

void FileSystemLayer::processAll() {
	wrapped->processAll();
}

}
