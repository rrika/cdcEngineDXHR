#pragma once

namespace cdc {

class PCDX11RenderDevice;

class PCDX11ConstantBufferPool { // guessed name
public:
	PCDX11RenderDevice *renderDevice; // 0
	// void *threadSafeMemHeapAllocator; // 4
	// uint32_t dword8;
	// uint32_t dwordC;
	// char *name;
	// uint32_t dword14;
	// uint32_t dword18;
};

}
