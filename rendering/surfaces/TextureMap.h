#pragma once
#include "../RenderResource.h"

namespace cdc {

class TextureMap : public RenderResource {
public:
	virtual uint32_t getWidth() = 0; // 18
	virtual uint32_t getHeight() = 0;
	// virtual void method_20() = 0;
	// virtual void method_24() = 0;
	// virtual void method_28() = 0;
	// virtual void method_2C() = 0;
};

}
