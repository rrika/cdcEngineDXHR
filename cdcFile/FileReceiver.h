#pragma once
#include <cstdint>

namespace cdc {

class FileRequest;

class FileReceiver {
public:
	virtual void process(FileRequest*, void *input, uint32_t size, uint32_t offset /*, uint32_t arg4*/) = 0; // 00
	virtual void requestStarted(FileRequest*, uint32_t size) = 0; // 04
	virtual void requestFailed(FileRequest*) = 0; // 08
	virtual void requestComplete(FileRequest*) = 0; // 0C
	virtual ~FileReceiver() = default;
};

}
