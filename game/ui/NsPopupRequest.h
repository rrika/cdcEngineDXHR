#pragma once
#include "cdcSys/RCObject.h"

class NsPopupRequest : public cdc::RCObject {
public:
	const char *m_title;
	const char *m_message;

	bool m_mouseSet = false; // HACK
public:
	NsPopupRequest(const char *title, const char *message);
};

// defined in fakescaleform/popups.cpp
void submitPopupRequest(NsPopupRequest *popup);
void buildPopupsMenu();
void buildPopupsUI();
