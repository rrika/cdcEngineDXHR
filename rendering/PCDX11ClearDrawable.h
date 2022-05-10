#pragma once
#include "IRenderDrawable.h"

namespace cdc {

class PCDX11RenderDevice;

class PCDX11ClearDrawable :
	public IRenderDrawable
{
public:
	PCDX11RenderDevice *renderDevice; // C
	uint32_t flags; // 10
	uint32_t clearStencil; // 14
	float clearDepth; // 18
	uint32_t clearColor; // 1C

	PCDX11ClearDrawable(
		PCDX11RenderDevice *renderDevice,
		uint32_t flags,
		uint32_t clearColor,
		float clearDepth,
		uint32_t clearStencil)
	:
		renderDevice(renderDevice),
		flags(flags),
		clearStencil(clearStencil),
		clearDepth(clearDepth),
		clearColor(clearColor)
	{}

	virtual void renderDrawable0();
	virtual uint32_t renderDrawable4();
};

}
