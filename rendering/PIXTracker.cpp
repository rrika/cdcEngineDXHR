#include "PIXTracker.h"
#include "PCDX11DeviceManager.h"

namespace cdc::PIXTracker {

void StartMarker(const char *str) {
	deviceManager->StartMarker(str);
}

void StartMarker(const wchar_t *wstr) {
	deviceManager->StartMarker(wstr);
}

void EndMarker() {
	deviceManager->EndMarker();
}

}
