#pragma once
#include "../types.h"

namespace cdc {

class RenderResource {
	virtual void method_00() = 0;
	virtual void fill(void* src, size_t size, size_t offset) = 0;
	virtual char *getBuffer() = 0;
	virtual void construct() = 0;
	virtual void method_10();
	virtual ~RenderResource() = 0;
};

}
