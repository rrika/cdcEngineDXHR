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

void PCDX11InternalResource::internalResource0C() { internalCreate(); }

void PCDX11InternalResource::internalResource10() { internalRelease(); }

void PCDX11InternalResource::internalOnSettingsChanged() { /* empty */ }

void PCDX11InternalResource::internalCreateIfDeviceManagerAgrees() {
	// if (!deviceManager11->byte30)
		internalCreate();
}

}
