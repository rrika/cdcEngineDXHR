#pragma once

namespace cdc {

class PCDX11RenderDevice;

class PCDX11DeferredUpdate {
protected:
	PCDX11DeferredUpdate *next = nullptr;
public:
	virtual void deferredUpdate(PCDX11RenderDevice*) = 0;
};

}
