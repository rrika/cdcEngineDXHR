#pragma once

namespace cdc {

class PCDX11DeferredUpdate {
protected:
	PCDX11DeferredUpdate *next = nullptr;
public:
	virtual void deferredUpdate() = 0;
};

}
