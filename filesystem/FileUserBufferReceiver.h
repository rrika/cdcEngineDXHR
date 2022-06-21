#pragma once
#include "FileReceiver.h"

namespace cdc {

class FileUserBufferReceiver : public FileReceiver {
	void *buffer;
	FileUserBufferReceiver(void *buffer) : buffer(buffer) {}
public:
	static FileUserBufferReceiver *create(void *buffer) { return new FileUserBufferReceiver(buffer); }
	void process(FileRequest*, void *input, uint32_t size, uint32_t offset) override;
	void requestFailed(FileRequest*) override;
	void requestComplete(FileRequest*) override;
};


}
