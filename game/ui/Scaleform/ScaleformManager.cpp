#include "game/Gameloop.h"
#include "ScaleformManager.h"

std::map<std::string, uint32_t> scaleformStringMap;

void ScaleformManager::Initialize() {
	for (uint32_t i=0; i<globalDatabase->numScaleformStrings; i++) {
		auto& entry = globalDatabase->scaleformStrings[i];
		scaleformStringMap[std::string(entry.string)] = entry.stringId;
	}
}
