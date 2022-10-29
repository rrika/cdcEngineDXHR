#pragma once
#include <cstdint>
#include "cdcSys/LinkedList.h"

namespace cdc {

class FileRequest;
class ResolveReceiver;
struct DRMReadDets;
struct ObjectTracker;

class ResolveObject {
public:
	FileRequest *fileRequest = nullptr; // 8
	DRMReadDets *drmReadDets = nullptr; // C
	const char *path; // 18
	uint32_t rootSection = ~0u; // 1C
	LinkedList<ResolveObject *> dependencies; // 20
	uint32_t numPendingDependencies = 0; // 24
	LinkedList<ResolveObject *> dependants; // 28
	ResolveReceiver *resolveReceiver = nullptr; // 38

	static ResolveObject *create(
		const char *path,
		void (*callback)(void*, void*, void*, ResolveObject*),
		void *callbackArg1,
		void *callbackArg2,
		void **outPtrWrapped,
		void (*unloadCallback)(ObjectTracker*, ResolveObject*),
		ObjectTracker *objectTracker,
		uint8_t y,
		uint32_t priority
	);

	static ResolveObject *create(
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
	);

	ResolveObject(const char *path) : path(path) {}

	void *getRootWrapped();
	void markForRetry(uint32_t missingDeps, ResolveReceiver *rr);
	void addDependency(ResolveObject *other);
	void addDependant(ResolveObject *other);
	void notifyDependants();
};

bool isLoaded(ResolveObject *);

}
