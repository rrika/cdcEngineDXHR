#include <cstdlib> // for getenv
#include "FileHelpers.h"
#include "ArchiveFileSystem.h"
#include "FileUserBufferReceiver.h"
#include "HackFileSystem.h"
#include "../sys/Assert.h"

namespace cdc {

FileSystem *lowerFileSystem;
FileSystem *threadedLowerFileSystem;

void createLowerFileSystems() {
	// TODO
	lowerFileSystem = new HackFileSystem();
	threadedLowerFileSystem = lowerFileSystem;
}

ArchiveFileSystem *archiveFileSystem_default;

void createHigherFileSystems() {
	const char *bigfilePath = getenv("BIGFILE");
	if (!bigfilePath) {
		printf("\nspecify path to BIGFILE.000 through BIGFILE environment variable\n\n");
		return;
	}

	archiveFileSystem_default = new ArchiveFileSystem(lowerFileSystem);
	bool indexOk = archiveFileSystem_default->readIndex(bigfilePath, 0);
	
	if (!indexOk)
		FatalError("Unable to open bigfile BIGFILE.DAT!");
}

void destroyFileSystems() {
	// TODO
}

FileSystem *getDefaultFileSystem() {
	// TODO
	return archiveFileSystem_default;
}

char *FSHelper_ReadFile(const char *path) {
	auto fs = archiveFileSystem_default; // HACK
	uint32_t size = fs->getSize(path);

	char *buffer = new char[size+1];
	FileReceiver *rec = FileUserBufferReceiver::create(buffer);
	FileRequest *req = fs->createRequest(rec, path, 0);
	req->submit(1);
	fs->processUntil(req);
	req->decrRefCount();
	// req is owned by fs which takes care of it in processAll()

	return buffer;
}

}
