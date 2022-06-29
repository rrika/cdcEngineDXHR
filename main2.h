#pragma once
#include "windows.h"
class ArchiveFileSystem;
extern void *yellowCursor;
extern ArchiveFileSystem *archiveFileSystem_default;
extern HWND hwnd1;
void setupPrefixes(const char *prefix);
void buildDRMPath(char *buffer, const char *name);
int WinMain2(HINSTANCE hInstance, LPSTR lpCmdLine);
