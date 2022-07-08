#include "ResolveObject.h"
#include "ResolveReceiver.h"
#include "../filesystem/ArchiveFileSystem.h"
#include "../main2.h"

// borrowed from ArchiveFileSystem.cpp
uint32_t pathCrc32(const char *path);

namespace cdc {

ResolveObject *ResolveObject::create(
	const char *path,
	void (*callback)(void*, void*, void*, ResolveObject*),
	void *callbackArg1,
	void *callbackArg2,
	void **outPtrWrapped,
	void (*unloadCallback)(PendingObject*, ResolveObject*),
	PendingObject *pendingObject,
	uint8_t y,
	uint32_t fsMethod18Arg
) {
	// uint32_t pathCrc = crc32_lowercase(path);
	uint32_t pathCrc = pathCrc32(path);
	return create(
		pathCrc,
		path,
		callback,
		callbackArg1,
		callbackArg2,
		outPtrWrapped,
		unloadCallback,
		pendingObject,
		y,
		fsMethod18Arg
	);
}

ResolveObject *ResolveObject::create(
	uint32_t pathCrc,
	const char *path,
	void (*callback)(void*, void*, void*, ResolveObject*),
	void *callbackArg1,
	void *callbackArg2,
	void **outPtrWrapped,
	void (*unloadCallback)(PendingObject*, ResolveObject*),
	PendingObject *pendingObject,
	uint8_t y,
	uint32_t fsMethod18Arg
) {
	// TODO: cache

	path = strdup(path); // HACK

	auto *resolveObject = new ResolveObject(path);
	auto *resolveReceiver = new ResolveReceiver(
		callback, callbackArg1, callbackArg2,
		outPtrWrapped, unloadCallback, pendingObject,
		resolveObject, y);

	// auto *fs = getFileSystem();
	auto *fs = archiveFileSystem_default; // HACK
	auto *req = fs->createRequest(resolveReceiver, path, 0);
	resolveObject->fileRequest = req;
	req->submit(fsMethod18Arg);
	// TODO

	return resolveObject;
}

}
