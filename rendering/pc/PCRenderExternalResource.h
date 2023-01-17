#pragma once
#include "rendering/RenderExternalResource.h"

namespace cdc {

class PCRenderDevice;

class PCRenderExternalResource : public RenderExternalResource {
public:
	PCRenderDevice *renderDevice;
	PCRenderExternalResource(PCRenderDevice *renderDevice)
		: renderDevice(renderDevice)
	{}
};

}
