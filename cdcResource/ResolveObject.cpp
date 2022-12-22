#include <cstring> // for strdup
#include <unordered_map>
#include "ResolveObject.h"
#include "ResolveReceiver.h"
#include "ResolveSection.h" // for g_resolveSections
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h"

// borrowed from ArchiveFileSystem.cpp
uint32_t pathCrc32(const char *path);

namespace cdc {

ResolveObject *ResolveObject::create(
	const char *path,
	void (*callback)(void*, void*, void*, ResolveObject*),
	void *callbackArg1,
	void *callbackArg2,
	void **outPtrWrapped,
	void (*unloadCallback)(ObjectTracker*, ResolveObject*),
	ObjectTracker *objectTracker,
	uint8_t y,
	uint32_t priority
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
		objectTracker,
		y,
		priority
	);
}

std::unordered_map<uint32_t, ResolveObject *> cachedObjects;

ResolveObject *ResolveObject::create(
	uint32_t pathCrc,
	const char *path,
	void (*callback)(void*, void*, void*, ResolveObject*),
	void *callbackArg1,
	void *callbackArg2,
	void **outPtrWrapped,
	void (*unloadCallback)(ObjectTracker*, ResolveObject*),
	ObjectTracker *objectTracker,
	uint8_t y,
	uint32_t priority
) {
	if (auto it = cachedObjects.find(pathCrc); it != cachedObjects.end()) {
		auto *resolveObject = it->second;

		if (outPtrWrapped && resolveObject->drmReadDets && resolveObject->drmReadDets->numDets)
			*outPtrWrapped = resolveObject->getRootWrapped();

		if (unloadCallback)
			if (auto rr = resolveObject->resolveReceiver) {
				rr->unloadCallback = unloadCallback;
				rr->objectTracker = objectTracker;
			}

		// TODO
		return resolveObject;
	}

	path = strdup(path); // HACK

	auto *resolveObject = new ResolveObject(path);
	cachedObjects[pathCrc] = resolveObject;
	auto *resolveReceiver = new ResolveReceiver(
		callback, callbackArg1, callbackArg2,
		outPtrWrapped, unloadCallback, objectTracker,
		resolveObject, y, &drmIndex);

	auto *fs = getDefaultFileSystem();
	auto *req = fs->createRequest(resolveReceiver, path, 0);
	resolveObject->fileRequest = req;
	req->submit(priority);
	// TODO

	return resolveObject;
}

void *ResolveObject::getRootWrapped() {
	if (rootSection == ~0u)
		return nullptr;
	DRMReadDet &det = drmReadDets->dets[rootSection];
	return g_resolveSections[det.contentType]->getWrapped(det.domainID);
}

void ResolveObject::markForRetry(uint32_t missingDeps, ResolveReceiver *rr) {
	(void)rr;
	numPendingDependencies = missingDeps;
	// byte34 = true;
	fileRequest->cancel();
	fileRequest = nullptr;
}

void ResolveObject::addDependency(ResolveObject *other) {
	dependencies.push_front(other);
}

void ResolveObject::addDependant(ResolveObject *other) {
	dependants.push_front(other);
}

void ResolveObject::notifyDependants() {
	for (auto *dependant : dependants) {
		if (--dependant->numPendingDependencies == 0) {
			printf("ready to load %s now (ro=%p, ro->rr=%p)\n", dependant->path, dependant, dependant->resolveReceiver);
			auto req = getDefaultFileSystem()->createRequest(
				dependant->resolveReceiver,
				dependant->path,
				0);
			dependant->fileRequest = req;
			req->submit(0);
			// dependant->byte34 = false;
		}
	}
}


bool isLoaded(ResolveObject *ro) {
	if (ro)
		return !ro->fileRequest; // && !ro->byte34;
	return true;
}

}
