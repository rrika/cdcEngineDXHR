#include "Scaleform/ScaleformManager.h"
#include "NsPopupRequest.h"

NsPopupRequest::NsPopupRequest(const char *title, const char *message) {
	m_title = ScaleformManager::LookupString(title);
	m_message = ScaleformManager::LookupString(message);
}
