#include <cstdio>
#include "cdcGameShell.h"
#include "cdcScript/GarbageCollector.h"
#include "cdcSound/Sound.h"

extern char pathPrefix[36];

namespace cdc {

bool GameShell::UpdateLoop(float timeInFrames) { // 572
	float timeInSeconds = timeInFrames * 0.03333333507180214;
	// TODO
	SOUND_Update(timeInSeconds);
	// TODO
	uint32_t numObjects = GarbageCollector::s_instance->NumObjects();
	if (numObjects < 50)
		numObjects = 50;
	GarbageCollector::s_instance->StartCollection();
	GarbageCollector::s_instance->Update(numObjects);
	// TODO
	return true;
}

void GameShell::LOAD_ObjectFileName(char *name, const char *object) { // line 752
	sprintf(name, "%s%s.drm", pathPrefix, object);
}

void GameShell::LOAD_UnitFileName(char *name, const char *unit) { // line 761
	sprintf(name, "%s%s.drm", pathPrefix, unit);
}

void GameShell::LOAD_StreamGroupFileName(char *name, const char *streamgroup) { // line 770
	sprintf(name, "%sStreamGroups\\%s.drm", pathPrefix, streamgroup);
}

void GameShell::LOAD_PadShockLibFileName(char *name, const char *libName) { // line 780
	sprintf(name, "%spadshock\\%s", pathPrefix, libName);
}

void GameShell::LOAD_MusicFileName(char *name, const char *music) { // line 785
	sprintf(name, "%smusic\\%s\\%s", pathPrefix, music, music); // twice?
}

void GameShell::LOAD_MiscFileName(char *name, const char *filename) { // line 800
	sprintf(name, "%s%s", pathPrefix, filename);
}

void GameShell::LOAD_IMFFileName(char *name, const char *filename) { // line 808
	sprintf(name, "%s%s", pathPrefix, filename);
}

}
