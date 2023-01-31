#include <cstring>
#include "FileUserBufferReceiver.h"

namespace cdc {

void FileUserBufferReceiver::process(FileRequest*, void *input, uint32_t size, uint32_t offset) {
	memcpy((char*)buffer + offset, input, size);
}

void FileUserBufferReceiver::requestStarted(FileRequest*, uint32_t size) {
	// empty
}

void FileUserBufferReceiver::requestFailed(FileRequest*) {
	delete this;
}

void FileUserBufferReceiver::requestComplete(FileRequest*) {
	delete this;
}

}
