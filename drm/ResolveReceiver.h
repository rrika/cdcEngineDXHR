#pragma once
#include <vector>

namespace cdc {

class FileSystem;

class ResolveSection;
void hackResolveReceiver(std::vector<char> data, ResolveSection **sections);
void hackResolveReceiver(FileSystem *fs, const char *path, ResolveSection **sections);

}
