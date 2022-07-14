#pragma once
#include "windows.h"
extern void *yellowCursor;
extern HWND hwnd1;
void setupPrefixes(const char *prefix);
void buildDRMPath(char *buffer, const char *name);
int WinMain2(HINSTANCE hInstance, LPSTR lpCmdLine);
