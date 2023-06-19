#include "Scaleform/ScaleformManager.h"
#include "NsPopupRequest.h"
#include "game/Impulse.h"

NsPopupRequest::NsPopupRequest(const char *title, const char *message) {
	m_title = ScaleformManager::LookupString(title);
	m_message = ScaleformManager::LookupString(message);
}

void NsPopupRequest::submit() {
	impulse_call_10Z(54, this); // handled in handlePopupRequest
}
