#include <cstdio>
#include "FileReceiver.h"
#include "HackFileSystem.h"

namespace cdc {

FileRequest *HackFile::createRequest(FileReceiver *receiver, const char *path, uint32_t offset) {
	return new HackFileRequest(fs, f, receiver, offset);
}

uint32_t HackFile::getSize() {
	fseek(f->f, 0, SEEK_END);
	size_t size = ftell(f->f);
	fseek(f->f, 0, SEEK_SET);
	return (uint32_t) size;
}


void HackFileRequest::incrRefCount() {
	refCount++;
}

void HackFileRequest::decrRefCount() {
	if (--refCount == 0)
		delete this;
}

void HackFileRequest::setCompressedSize(uint32_t) {
	// empty
}

void HackFileRequest::setReadAmount(uint32_t size) {
	this->size = size;
}

uint32_t HackFileRequest::getCompletionStatus() {
	return completionStatus;	
}

void HackFileRequest::submit(uint8_t priority) {
	fs->requests.push_back(this);
}

void HackFileRequest::cancel() {
	completionStatus = 4;
}



FileRequest *HackFileSystem::createRequest(FileReceiver *receiver, const char *path, uint32_t offset) {
	FILE *f = fopen(path, "rb");
	if (!f)
		return nullptr;
	HackFile hf(this, std::make_shared<FileWrapper>(f));
	uint32_t size = hf.getSize();
	auto *req = hf.createRequest(receiver, path, offset);
	req->setReadAmount(size);
	return req;
	// return new HackFileRequest(this, std::make_shared<FileWrapper>(f), receiver, offset);
}

File *HackFileSystem::createFile(const char *path) {
	FILE *f = fopen(path, "rb");
	if (f == nullptr)
		return nullptr;
	return new HackFile(this, std::make_shared<FileWrapper>(f));
}

bool HackFileSystem::FileExists(const char *path) {
	// TODO
	return true;
}

uint32_t HackFileSystem::getSize(const char *path) {
	FILE *f = fopen(path, "rb");
	HackFile hf(this, std::make_shared<FileWrapper>(f));
	return hf.getSize();
}

void HackFileSystem::setLanguageMask(uint32_t) {
	// empty
}

uint32_t HackFileSystem::getLanguageMask() {
	return 0;
}

bool HackFileSystem::hasRequests() {
	return !requests.empty();
}

void HackFileSystem::processRequest() {
	if (requests.empty())
		return;
	HackFileRequest *req = requests.front();
	requests.erase(requests.begin());

	printf("processing request file %p, %d+%d -> %p\n", req->f->f, req->offset, req->size, req->receiver);
	char *buffer = new char[req->size];
	fseek(req->f->f, req->offset, SEEK_SET);
	fread(buffer, req->size, 1, req->f->f);
	req->receiver->requestStarted(req, req->size);
	req->receiver->process(req, buffer, req->size, 0);

	if (req->completionStatus == 4)
		req->receiver->requestFailed(req);
	else {
		req->receiver->requestComplete(req);
		req->completionStatus = 3;
	}

	delete[] buffer;
	req->decrRefCount();
}

}