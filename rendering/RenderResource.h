#pragma once
#include <cstdint>

namespace cdc {

class RenderResource {
public:
	virtual void resFree() = 0;       // 0, Release
	virtual void resFill(void* src, uint32_t size, uint32_t offset) = 0; // 4, AddResourceData
	virtual char *resGetBuffer() = 0; // 8, GetResourceData
	virtual void resConstruct() = 0;  // C, FinalizeInitialization
	virtual void resMethod10();       // 10, UpdateResource
	virtual ~RenderResource() = 0;    // 14 
};

}
