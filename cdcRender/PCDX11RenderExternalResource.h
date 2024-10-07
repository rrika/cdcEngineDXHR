#pragma once
#include "RenderExternalResource.h"

namespace cdc {

class PCDX11RenderDevice;

class PCDX11RenderExternalResource : public RenderExternalResource {
public:
	PCDX11RenderDevice *renderDevice;
	PCDX11RenderExternalResource(PCDX11RenderDevice *renderDevice)
		: renderDevice(renderDevice)
	{}
};

}
