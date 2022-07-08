#pragma once
#include <cstdint>

namespace cdc {

class FileRequest;
class ResolveReceiver;
struct DRMReadDets;
struct PendingObject;

class ResolveObject {
public:
	FileRequest *fileRequest = nullptr; // 8
	DRMReadDets *drmReadDets = nullptr; // C
	const char *path; // 18
	uint32_t rootSection = ~0u; // 1C
	ResolveReceiver *resolveReceiver = nullptr; // 38

	static ResolveObject *create(
		const char *path,
		void (*callback)(void*, void*, void*, ResolveObject*),
		void *callbackArg1,
		void *callbackArg2,
		void **outPtrWrapped,
		void (*unloadCallback)(PendingObject*, ResolveObject*),
		PendingObject *pendingObject,
		uint8_t y,
		uint32_t fsMethod18Arg
	);

	static ResolveObject *create(
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
	);

	ResolveObject(const char *path) : path(path) {}
};

}
