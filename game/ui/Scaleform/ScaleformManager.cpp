#include "ScaleformManager.h"
#include "game/Gameloop.h"
#include "cdcLocale/localstr.h"

std::map<std::string, uint32_t> scaleformStringMap;

void ScaleformManager::Initialize() {
	for (uint32_t i=0; i<globalDatabase->numScaleformStrings; i++) {
		auto& entry = globalDatabase->scaleformStrings[i];
		scaleformStringMap[std::string(entry.string)] = entry.stringId;
	}
}

const char *ScaleformManager::LookupString(const char *str) {
	uint32_t i = scaleformStringMap[std::string(str)];
	return localstr_get(i);
}
