#pragma once

namespace cdc {

class PCInternalResource {
	PCInternalResource *prev;
	PCInternalResource *next;
public:
	PCInternalResource();
	virtual ~PCInternalResource();
	virtual bool internalCreate() = 0;
	virtual void internalRelease() = 0;
	virtual bool internalResource0C();
	virtual void internalResource10();
	virtual void internalOnSettingsChanged();

	void internalCreateIfDeviceManagerAgrees();
	void OnCreateResourceFailed();
};

}
