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
	ResolveObject *resolveObject;

public:
	ResolveReceiver(ResolveObject *object, DRMIndex *index, decltype(callback) callback=nullptr, void *arg1=nullptr, void *arg2=nullptr) :
		resolveObject(object), index(index), callback(callback), callbackArg1(arg1), callbackArg2(arg2) {}
	void process(FileRequest*, void *input, uint32_t size, uint32_t offset) override;
	void requestFailed(FileRequest*) override;
	void requestComplete(FileRequest*) override;
};

}
