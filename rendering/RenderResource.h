#pragma once
#include <cstdint>

namespace cdc {

class RenderResource {
public:
	virtual void resRegisterAtRenderDevice() = 0;
	virtual void resFill(void* src, uint32_t size, uint32_t offset) = 0;
	virtual char *resGetBuffer() = 0;
	virtual void resConstruct() = 0;
	virtual void resMethod10();
	virtual ~RenderResource() = 0;
};

}
