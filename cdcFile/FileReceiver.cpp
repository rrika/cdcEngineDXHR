#include <cstring>
#include "FileReceiver.h"

namespace cdc {

void FileAllocatedReceiver::process(FileRequest*, void *input, uint32_t size, uint32_t offset) {
	memcpy((char*)m_pBuffer + offset, input, size);
}

}
