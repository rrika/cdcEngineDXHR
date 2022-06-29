#pragma once
#include <cstdint>

namespace cdc {

class FileRequest;
class ResolveReceiver;
struct DRMReadDets;

class ResolveObject {
public:
	FileRequest *fileRequest = nullptr; // 8
	DRMReadDets *drmReadDets = nullptr; // C
	const char *path; // 18
	uint32_t rootSection = ~0u; // 1C
	ResolveReceiver *resolveReceiver = nullptr; // 38

	ResolveObject(const char *path) : path(path) {}
};

}
