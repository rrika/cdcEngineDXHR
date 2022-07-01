#pragma once
#include <vector>
#include "DRMIndex.h"
#include "../filesystem/FileReceiver.h"

namespace cdc {

class FileSystem;
class ResolveObject;

class ResolveSection;
std::vector<DRMSectionHeader> hackResolveReceiver(std::vector<char> data, ResolveSection **sections);
void hackResolveReceiver(FileSystem *fs, const char *path, ResolveSection **sections, ResolveObject *resolveObject, DRMIndex *index=nullptr);

class ResolveReceiver : public FileReceiver {
	// HACK: instead of parsing the DRM incrementally, everything goes into this buffer first
	std::vector<char> buffer;
	DRMIndex *index; // custom addition

	void (*callback)(void*, void*, void*, ResolveObject*);
	void *callbackArg1;
	void *callbackArg2;
	void **rootSectionPtr;
	ResolveObject *resolveObject;

public:
	ResolveReceiver(
		decltype(callback) callback, void *arg1, void *arg2,
		void **rootSectionPtr,
		ResolveObject *object,
		DRMIndex *index)
	:
		callback(callback), callbackArg1(arg1), callbackArg2(arg2), rootSectionPtr(rootSectionPtr),
		resolveObject(object), index(index)
	{}
	void process(FileRequest*, void *input, uint32_t size, uint32_t offset) override;
	void requestFailed(FileRequest*) override;
	void requestComplete(FileRequest*) override;
};

}
