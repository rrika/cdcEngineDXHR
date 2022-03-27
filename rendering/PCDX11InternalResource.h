#pragma once

namespace cdc {

class PCDX11InternalResource {
	PCDX11InternalResource *prev;
	PCDX11InternalResource *next;
public:
	PCDX11InternalResource();
	virtual ~PCDX11InternalResource();
	virtual void internalResource04() = 0;
	virtual void internalResource08() = 0;
	virtual void internalResource0C();
	virtual void internalResource10();
	virtual void internalResource14();
};

}
