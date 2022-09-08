#include <cstdio>
#include "cdcGameShell.h"

extern char pathPrefix[36];

namespace cdc {

void GameShell::LOAD_ObjectFileName(char *name, const char *object) {
	sprintf(name, "%s%s.drm", pathPrefix, object);
}

void GameShell::LOAD_UnitFileName(char *name, const char *unit) {
	sprintf(name, "%s%s.drm", pathPrefix, unit);
}

void GameShell::LOAD_StreamGroupFileName(char *name, const char *streamgroup) {
	sprintf(name, "%sStreamGroups\\%s.drm", pathPrefix, streamgroup);
}

void GameShell::LOAD_PadShockLibFileName(char *name, const char *libName) {
	sprintf(name, "%spadshock\\%s", pathPrefix, libName);
}

void GameShell::LOAD_MusicFileName(char *name, const char *music) {
	sprintf(name, "%smusic\\%s\\%s", pathPrefix, music, music); // twice?
}

void GameShell::LOAD_MiscFileName(char *name, const char *filename) {
	sprintf(name, "%s%s", pathPrefix, filename);
}

void GameShell::LOAD_IMFFileName(char *name, const char *filename) {
	sprintf(name, "%s%s", pathPrefix, filename);
}

}
