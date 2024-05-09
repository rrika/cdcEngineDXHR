#include <cstring> // for strdup
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
	void (*cancelCallback)(ObjectTracker*, ResolveObject*),
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
		cancelCallback,
		objectTracker,
		y,
		priority
	);
}

std::map<uint32_t, ResolveObject *> cachedObjects; // map with stable iterators

ResolveObject *ResolveObject::create(
	uint32_t pathCrc,
	const char *path,
	void (*callback)(void*, void*, void*, ResolveObject*),
	void *callbackArg1,
	void *callbackArg2,
	void **outPtrWrapped,
	void (*cancelCallback)(ObjectTracker*, ResolveObject*),
	ObjectTracker *objectTracker,
	uint8_t y,
	uint32_t priority
) {
	if (auto it = cachedObjects.find(pathCrc); it != cachedObjects.end()) {
		auto *resolveObject = it->second;

		if (outPtrWrapped && resolveObject->m_pRecord && resolveObject->m_pRecord->m_numEntries)
			*outPtrWrapped = resolveObject->getRootWrapped();

		if (cancelCallback)
			if (auto rr = resolveObject->resolveReceiver) {
				rr->cancelCallback = cancelCallback;
				rr->objectTracker = objectTracker;
			}

		resolveObject->m_ref++;
		return resolveObject;
	}

	path = strdup(path); // HACK

	auto it = cachedObjects.insert({pathCrc, nullptr}).first;
	auto *resolveObject = new ResolveObject(path, it);
	it->second = resolveObject;
	auto *resolveReceiver = new ResolveReceiver(
		callback, callbackArg1, callbackArg2,
		outPtrWrapped, cancelCallback, objectTracker,
		resolveObject, y, &drmIndex);

	auto *fs = getDefaultFileSystem();
	auto *req = fs->createRequest(resolveReceiver, path, 0);
	resolveObject->fileRequest = req;
	req->submit(priority);

	resolveObject->m_ref++;
	return resolveObject;
}

ResolveObject::~ResolveObject() {
	if (m_it != cachedObjects.end())
		cachedObjects.erase(m_it);
	for (auto *dependency : dependencies) {
		dependency->Release();
	}
}


void *ResolveObject::getRootWrapped() {
	if (rootSection == ~0u)
		return nullptr;
	SectionRecord::Entry &entry = m_pRecord->m_pEntry[rootSection];
	return g_resolveSections[entry.contentType]->GetBasePointer(entry.domainID);
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

int32_t ResolveObject::Release() { // returns a refcount
	if (m_ref == 1) {
		if (fileRequest)
			fileRequest->cancel();
		fileRequest = nullptr;
	}

	if (--m_ref > 0)
		return m_ref; // not time to unload yet

	if (m_pRecord) {
		m_pRecord->Release(this); // deletes itself
		m_pRecord = nullptr;
	}
	if (resolveReceiver) {
		resolveReceiver->Cancel();
		if (/*byte34_or_ownsResolveReceiver &&*/ resolveReceiver) // checked again apparently
			delete resolveReceiver;
	}
	resolveReceiver = nullptr;
	if (m_ref >= 0) // unsure why, it's not like this can be called multiple times
		delete this;

	return 0;
}


bool isLoaded(ResolveObject *ro) {
	if (ro)
		return !ro->fileRequest; // && !ro->byte34;
	return true;
}

}
