#pragma once
#include <vector>

namespace cdc {

class ResolveSection;
void hackResolveReceiver(std::vector<char> data, ResolveSection **sections);
void hackResolveReceiver(const char *path, ResolveSection **sections);

}
