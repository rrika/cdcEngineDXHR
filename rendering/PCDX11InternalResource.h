#pragma once

namespace cdc {

class PCDX11InternalResource {
	PCDX11InternalResource *prev;
	PCDX11InternalResource *next;
public:
	PCDX11InternalResource();
	virtual ~PCDX11InternalResource();
	virtual bool internalCreate() = 0;
	virtual void internalRelease() = 0;
	virtual void internalResource0C();
	virtual void internalResource10();
	virtual void internalOnSettingsChanged();

	void internalCreateIfDeviceManagerAgrees();
};

}
