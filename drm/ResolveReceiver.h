#pragma once
#include <vector>
#include "DRMIndex.h"
#include "../filesystem/FileReceiver.h"

namespace cdc {

class FileSystem;
class ResolveObject;
struct PendingObject;

class ResolveSection;
std::vector<DRMSectionHeader> hackResolveReceiver(std::vector<char> data, ResolveSection **sections, ResolveObject *resolveObject, bool requestDependencies=true);
void hackResolveReceiver(FileSystem *fs, const char *path, ResolveSection **sections, ResolveObject *resolveObject, DRMIndex *index=nullptr);

class ResolveReceiver : public FileReceiver {
	// HACK: instead of parsing the DRM incrementally, everything goes into this buffer first
	std::vector<char> buffer;
	DRMIndex *index; // custom addition

	ResolveObject *resolveObject; // 4
	void (*callback)(void*, void*, void*, ResolveObject*); // 6C
	void *callbackArg1; // 70
	void *callbackArg2; // 74
	void **rootSectionPtr; // 78
	void (*unloadCallback)(PendingObject*, ResolveObject*); // 7C
	PendingObject *pendingObject; // 80

	friend class ResolveObject; // need to mess with unloadCallback etc.

public:
	ResolveReceiver(
		decltype(callback) callback, void *arg1, void *arg2,
		void **rootSectionPtr,
		void (*unloadCallback)(PendingObject*, ResolveObject*),
		PendingObject *pendingObject,
		ResolveObject *resolveObject,
		uint8_t unknown,
		DRMIndex *index = nullptr)
	:
		callback(callback), callbackArg1(arg1), callbackArg2(arg2), rootSectionPtr(rootSectionPtr),
		unloadCallback(unloadCallback), pendingObject(pendingObject),
		resolveObject(resolveObject), index(index)
	{
		resolveObject->resolveReceiver = this;
	}
	void process(FileRequest*, void *input, uint32_t size, uint32_t offset) override;
	void requestFailed(FileRequest*) override;
	void requestComplete(FileRequest*) override;
};

}
