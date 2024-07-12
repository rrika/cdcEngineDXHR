#pragma once
#include <cstdint>
#include "cdcSys/SList.h"
#include <map>

namespace cdc {

class FileRequest;
class ResolveReceiver;
struct ObjectTracker;
struct SectionRecord;

class ResolveObject {
public:
	FileRequest *fileRequest = nullptr; // 8
	SectionRecord *m_pRecord = nullptr; // C
	int32_t m_ref = 0; // 14
	const char *path; // 18
	uint32_t rootSection = ~0u; // 1C
	SList<ResolveObject *> dependencies; // 20
	uint32_t numPendingDependencies = 0; // 24
	SList<ResolveObject *> dependants; // 28
	std::map<uint32_t, ResolveObject*>::iterator m_it; // 2C
	ResolveReceiver *resolveReceiver = nullptr; // 38
		// set by ResolveReceiver ctor

	static ResolveObject *create(
		const char *path,
		void (*callback)(void*, void*, void*, ResolveObject*),
		void *callbackArg1,
		void *callbackArg2,
		void **outPtrWrapped,
		void (*cancelCallback)(ObjectTracker*, ResolveObject*),
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
		void (*cancelCallback)(ObjectTracker*, ResolveObject*),
		ObjectTracker *objectTracker,
		uint8_t y,
		uint32_t priority
	);

	static ResolveObject *find(const char *path);

	ResolveObject(const char *path, decltype(m_it) it) : path(path), m_it(it) {}
	~ResolveObject();

	void *getRootWrapped();
	void markForRetry(uint32_t missingDeps, ResolveReceiver *rr);
	void addDependency(ResolveObject *other);
	void addDependant(ResolveObject *other);
	void notifyDependants();
	int32_t Release();
};

bool isLoaded(ResolveObject *);

}
