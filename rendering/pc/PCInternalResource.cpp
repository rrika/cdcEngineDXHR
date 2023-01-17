#include "PCInternalResource.h"

namespace cdc {

PCInternalResource::PCInternalResource() :
	prev((PCInternalResource*)-1),
	next((PCInternalResource*)-1)
{
	// deviceManager9->register(this);
}

PCInternalResource::~PCInternalResource() {
	// deviceManager9->unregister(this);
}

bool PCInternalResource::internalResource0C() { return internalCreate(); }

void PCInternalResource::internalResource10() { internalRelease(); }

void PCInternalResource::internalOnSettingsChanged() { /* empty */ }

void PCInternalResource::internalCreateIfDeviceManagerAgrees() {
	// if (!deviceManager9->dword10 && !deviceManager9->byte158)
		internalCreate();
}

}
