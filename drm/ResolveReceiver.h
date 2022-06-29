#pragma once
#include <vector>
#include "DRMIndex.h"
#include "../filesystem/FileReceiver.h"

namespace cdc {

class FileSystem;

class ResolveSection;
std::vector<DRMSectionHeader> hackResolveReceiver(std::vector<char> data, ResolveSection **sections);
void hackResolveReceiver(FileSystem *fs, const char *path, ResolveSection **sections, DRMIndex *index=nullptr);

class ResolveReceiver : public FileReceiver {
	// HACK: instead of parsing the DRM incrementally, everything goes into this buffer first
	std::vector<char> buffer;
	const char *path;
	DRMIndex *index;
public:
	ResolveReceiver(const char *path, DRMIndex *index) : path(path), index(index) {}
	void process(FileRequest*, void *input, uint32_t size, uint32_t offset) override;
	void requestFailed(FileRequest*) override;
	void requestComplete(FileRequest*) override;
};

}
