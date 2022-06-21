#include "FileSystemLayer.h"

namespace cdc {

FileRequest *FileSystemLayer::createRequest(FileReceiver *receiver, const char *path, uint32_t offset) {
	return wrapped->createRequest(receiver, path, offset);
}

File *FileSystemLayer::createFile(const char *path) {
	return wrapped->createFile(path);
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
