#pragma once
#include "windows.h"
#include "config.h"
extern void *yellowCursor;
extern HWND hwnd1;
void setupPrefixes(const char *prefix);
void buildDRMPath(char *buffer, const char *name);

#if ENABLE_MCE
extern bool relaunchMce;
#endif
