#include "PCDX11InternalResource.h"

namespace cdc {

PCDX11InternalResource::PCDX11InternalResource() :
	prev((PCDX11InternalResource*)-1),
	next((PCDX11InternalResource*)-1)
{
	// deviceManager->register(this);
}

PCDX11InternalResource::~PCDX11InternalResource() {
	// deviceManager->unregister(this);
}

void PCDX11InternalResource::internalResource0C() { internalResource04(); }

void PCDX11InternalResource::internalResource10() { internalResource08(); }

void PCDX11InternalResource::internalResource14() { /* empty */ }

}
