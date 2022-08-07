#include <cstdio>
#include <cstring>
#include "Main.h"

char buildType[16];
char pathPrefix[36];
const char *audioPrefix;
const char *cinematicPrefix;

void SetupBuildDir(const char *prefix) {
	strcpy(buildType, prefix);
	sprintf(pathPrefix, "%s\\", prefix);
	// TODO
}

void buildDRMPath(char *buffer, const char *name) {
	sprintf(buffer, "%s%s.drm", pathPrefix, name);
}
